/**
 * @file PathIntegrator-new.cpp
 * @author Chenxi Zhou
 * @brief Path Integrator with new implementations
 * @version 0.1
 * @date 2022-09-21
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#include "CoreLayer/Geometry/CoordConvertor.h"
#include "PathIntegrator-new.h"

PathIntegratorNew::PathIntegratorNew(std::shared_ptr<Camera> _camera,
                                     std::unique_ptr<Film> _film,
                                     std::unique_ptr<TileGenerator> _tileGenerator,
                                     std::shared_ptr<Sampler> _sampler,
                                     int _spp,
                                     int _renderThreadNum) : 
    AbstractPathIntegrator(_camera, 
                           std::move(_film), 
                           std::move(_tileGenerator), 
                           _sampler, _spp, 
                           _renderThreadNum)
{

}

Spectrum PathIntegratorNew::Li(const Ray &initialRay, 
                               std::shared_ptr<Scene> scene) 
{
    Spectrum L{.0};
    Spectrum throughput{1.0};
    Ray ray = initialRay;
    int nBounces = 0;
    auto itsOpt = scene->intersect(ray);
    PathIntegratorLocalRecord evalLightRecord = evalEmittance(scene, itsOpt, ray);

    while (true) {

        //* All rays generated by bsdf/phase sampling which might hold radiance
        //* will be counted at the end of the loop
        //* except the ray generated by camera which is considered here
        if (nBounces == 0) {
            if (!evalLightRecord.f.isBlack()) {
                L += throughput * evalLightRecord.f;
            }    
        }

        //* No intersection
        if (!itsOpt.has_value())
            break;
        
        auto its = itsOpt.value();

        nBounces++;
        double pSurvive = russianRoulette(scene, its, throughput, nBounces);
        if (randFloat() > pSurvive)
            break;
        throughput /= pSurvive;

        //* ----- Direct Illumination -----
        for (int i = 0; i < nDirectLightSamples; ++i) {
            PathIntegratorLocalRecord sampleLightRecord = sampleDirectLighting(scene, its, ray);
            PathIntegratorLocalRecord evalScatterRecord = evalScatter(scene, its, ray, sampleLightRecord.wi);

            if (!sampleLightRecord.f.isBlack()) {
                //* Multiple importance sampling
                double misw = MISWeight(sampleLightRecord.pdf, evalScatterRecord.pdf);
                if (sampleLightRecord.isDelta) {
                    misw = 1.0;
                }
                L += throughput * sampleLightRecord.f * evalScatterRecord.f 
                     / sampleLightRecord.pdf * misw
                     / nDirectLightSamples;
            }
        }

        //*----- BSDF Sampling -----
        PathIntegratorLocalRecord sampleScatterRecord = sampleScatter(scene, its, ray);
        if (!sampleScatterRecord.f.isBlack()) {
            throughput *= sampleScatterRecord.f / sampleScatterRecord.pdf;
        } else {
            break;
        }

        //* Test whether the bsdf sampling ray hit the emitter
        const double eps = 1e-4;
        ray = Ray{its.position + sampleScatterRecord.wi * eps, sampleScatterRecord.wi};
        itsOpt = scene->intersect(ray);

        evalLightRecord = evalEmittance(scene, itsOpt, ray);
        if (!evalLightRecord.f.isBlack()) {
            //* The continuous ray hit the emitter
            //* Multiple importance sampling

            double misw = MISWeight(sampleScatterRecord.pdf, evalLightRecord.pdf);
            if (sampleScatterRecord.isDelta) {
                misw = 1.0;
            }

            L += throughput * evalLightRecord.f * misw;
        }
        
    }

    return L;

}

PathIntegratorLocalRecord PathIntegratorNew::evalEmittance(std::shared_ptr<Scene> scene, 
                                                           std::optional<Intersection> itsOpt, 
                                                           const Ray &ray)
{
    Vec3d wo = -ray.direction;
    Spectrum LEmission(0.0);
    double pdfDirect = 1.0;
    if (!itsOpt.has_value())
    {
        auto record = evalEnvLights(scene, ray);
        LEmission = record.f;
        pdfDirect = record.pdf;
    }
    else if (itsOpt.value().object && itsOpt.value().object->getLight())
    {
        auto its = itsOpt.value();
        Normal3d n = its.geometryNormal;
        auto light = itsOpt.value().object->getLight();
        auto record = light->eval(ray, its, ray.direction);
        LEmission = record.s;
        Intersection tmpIts;
        tmpIts.position = ray.origin;
        pdfDirect = record.pdfDirect * chooseOneLightPdf(scene, tmpIts, ray, light);
    }
    Spectrum transmittance(1.0); // todo: transmittance eval
    return {ray.direction, transmittance * LEmission, pdfDirect, false}; 

}

PathIntegratorLocalRecord PathIntegratorNew::sampleDirectLighting(std::shared_ptr<Scene> scene, 
                                                                  const Intersection &its, 
                                                                  const Ray &ray)
{
    auto [light, pdfChooseLight] = chooseOneLight(scene, its, ray, sampler->sample1D());
    auto record = light->sampleDirect(its, sampler->sample2D(), ray.timeMin);
    double pdfDirect = record.pdfDirect * pdfChooseLight; // pdfScatter with respect to solid angle
    Vec3d dirScatter = record.wi;
    Spectrum Li = record.s;
    Point3d posL = record.dst;
    Point3d posS = its.position;
    Spectrum transmittance(1.0); // todo: transmittance eval
    Ray visibilityTestingRay(posL - dirScatter * 1e-4, -dirScatter, ray.timeMin, ray.timeMax);
    auto visibilityTestingIts = scene->intersect(visibilityTestingRay);
    if (!visibilityTestingIts.has_value() || visibilityTestingIts->object != its.object || (visibilityTestingIts->position - posS).length2() > 1e-6)
    {
        transmittance = 0.0;
    }
    return {dirScatter, Li * transmittance, pdfDirect, record.isDeltaPos};

}

PathIntegratorLocalRecord PathIntegratorNew::evalScatter(std::shared_ptr<Scene> scene,
                                                      const Intersection &its,
                                                      const Ray &ray,
                                                      const Vec3d &dirScatter)
{
    if (its.material != nullptr)
    {
        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
        Normal3d n = its.geometryNormal;
        double wiDotN = std::abs(dot(n, dirScatter));
        Vec3d wi = its.toLocal(dirScatter);
        Vec3d wo = its.toLocal(-ray.direction);
        return {
            dirScatter,
            bxdf->f(wo, wi) * wiDotN,
            bxdf->pdf(wo, wi),
            false};
    }
    else
    {
        // todo: eval phase function
        return {};
    }
}

PathIntegratorLocalRecord PathIntegratorNew::sampleScatter(std::shared_ptr<Scene> scene,
                                                        const Intersection &its,
                                                        const Ray &ray)
{
    if (its.material != nullptr)
    {
        Vec3d wo = its.toLocal(-ray.direction);
        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
        Vec3d n = its.geometryNormal;
        BxDFSampleResult bsdfSample = bxdf->sample(wo, sampler->sample2D());
        double pdf = bsdfSample.pdf;
        Vec3d dirScatter = its.toWorld(bsdfSample.directionIn);
        double wiDotN = std::abs(dot(dirScatter, n));
        return {dirScatter, bsdfSample.s * wiDotN, pdf, BxDF::MatchFlags(bsdfSample.bxdfSampleType,BXDF_SPECULAR)};
    }
    else
    {
        // todo: sample phase function
        return {};
    }
}

double PathIntegratorNew::russianRoulette(std::shared_ptr<Scene> scene,
                                       const Intersection &its,
                                       const Spectrum &throughput,
                                       int nBounce)
{
    // double pSurvive = std::min(pRussianRoulette, throughput.sum());
    double pSurvive = pRussianRoulette;
    if (nBounce > nPathLengthLimit)
        pSurvive = 0.0;
    if (nBounce <= 2)
        pSurvive = 1.0;
    return pSurvive;
}

std::pair<std::shared_ptr<Light>, double> 
PathIntegratorNew::chooseOneLight(std::shared_ptr<Scene> scene,
                                  const Intersection &its,
                                  const Ray &ray,
                                  double lightSample)
{
    // uniformly weighted
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    int numLights = lights->size();
    int lightID = std::min(numLights - 1, (int)(lightSample * numLights));
    std::shared_ptr<Light> light = lights->operator[](lightID);
    return {light, 1.0 / numLights};
}

double PathIntegratorNew::chooseOneLightPdf(std::shared_ptr<Scene> scene,
                                            const Intersection &its,
                                            const Ray &ray,
                                            std::shared_ptr<Light> light)
{
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    int numLights = lights->size();
    return 1.0 / numLights;
}

PathIntegratorLocalRecord PathIntegratorNew::evalEnvLights(std::shared_ptr<Scene> scene,
                                                           const Ray &ray)
{
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    Spectrum L(0.0);
    double pdf = 0.0;
    for (auto light : *lights)
    {
        auto record = light->evalEnvironment(ray);
        L += record.s;
        pdf += record.pdfEmitDir;
    }
    return {-ray.direction, L, pdf};
}