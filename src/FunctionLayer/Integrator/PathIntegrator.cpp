/**
 * @file PathIntegrator.cpp
 * @author Zhimin Fan
 * @brief Path Integrator
 * @version 0.1
 * @date 2022-05-06
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "PathIntegrator.h"
#include "FastMath.h"

PathIntegrator::PathIntegrator(
        std::shared_ptr<Camera> _camera, 
        std::unique_ptr<Film> _film, 
        std::unique_ptr<TileGenerator> _tileGenerator, 
        std::shared_ptr<Sampler> _sampler, 
        int _spp,
        int _renderThreadNum):
            AbstractPathIntegrator(_camera,std::move(_film),std::move(_tileGenerator),_sampler,_spp,_renderThreadNum)
{

}

PathIntegratorLocalRecord PathIntegrator::evalEmittance(std::shared_ptr<Scene> scene,
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
        pdfDirect = record.pdfDirect * chooseOneLightPdf(scene, light);
    }
    Spectrum transmittance(1.0); // todo: transmittance eval
    return {ray.direction, transmittance * LEmission, pdfDirect, false}; 
}

PathIntegratorLocalRecord PathIntegrator::sampleDirectLighting(std::shared_ptr<Scene> scene,
                                                               const Intersection &its,
                                                               const Ray &ray)
{
    auto [light, pdfChooseLight] = chooseOneLight(scene, sampler->sample1D());
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
    if(!visibilityTestingIts.has_value() && light->lightType==ELightType::INFINITE){
        transmittance = 1.0;
    }
    return {dirScatter, Li * transmittance, pdfDirect, record.isDeltaPos};
}

PathIntegratorLocalRecord PathIntegrator::evalScatter(const Intersection &its,
                                                      const Ray &ray,
                                                      const Vec3d &dirScatter)
{
    if (its.material != nullptr)
    {
        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
        Normal3d n = its.geometryNormal;
        double wiDotN = fm::abs(dot(n, dirScatter));
        Vec3d wi = its.toLocal(dirScatter);
        Vec3d wo = its.toLocal(-ray.direction);
        return {
            dirScatter,
            bxdf->f(wo, wi,false) * wiDotN,
            bxdf->pdf(wo, wi),
            false};
    }
    else
    {
        // todo: eval phase function
        return {};
    }
}

PathIntegratorLocalRecord PathIntegrator::sampleScatter(const Intersection &its,
                                                        const Ray &ray)
{
    if (its.material != nullptr)
    {
        Vec3d wo = its.toLocal(-ray.direction);
        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
        Vec3d n = its.geometryNormal;
        BxDFSampleResult bsdfSample = bxdf->sample(wo, sampler->sample2D(),false);
        double pdf = bsdfSample.pdf;
        Vec3d dirScatter = its.toWorld(bsdfSample.directionIn);
        double wiDotN = fm::abs(dot(dirScatter, n));
        return {dirScatter, bsdfSample.s * wiDotN, pdf, BxDF::MatchFlags(bsdfSample.bxdfSampleType,BXDF_SPECULAR)};
    }
    else
    {
        // todo: sample phase function
        return {};
    }
}

double PathIntegrator::russianRoulette(const Spectrum &throughput,
                                       int nBounce)
{
    // double pSurvive = std::min(pRussianRoulette, throughput.sum());
    double pSurvive = pRussianRoulette;
    if (nBounce > nPathLengthLimit)
        pSurvive = 0.0;
    if (nBounce <= 20)
        pSurvive = 1.0;
    return pSurvive;
}

std::pair<std::shared_ptr<Light>, double> PathIntegrator::chooseOneLight(std::shared_ptr<Scene> scene,
                                                                         double lightSample)
{
    // uniformly weighted
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    int numLights = lights->size();
    int lightID = std::min(numLights - 1, (int)(lightSample * numLights));
    std::shared_ptr<Light> light = lights->operator[](lightID);
    return {light, 1.0 / numLights};
}

double PathIntegrator::chooseOneLightPdf(std::shared_ptr<Scene> scene,
                                         std::shared_ptr<Light> light)
{
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    int numLights = lights->size();
    return 1.0 / numLights;
}

PathIntegratorLocalRecord PathIntegrator::evalEnvLights(std::shared_ptr<Scene> scene,
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