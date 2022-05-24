/**
 * @file PathIntegrator.cpp
 * @author Zhimin Fan
 * @brief Path Integrator
 * @version 0.1
 * @date 2022-05-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "PathIntegrator.h"

PathIntegratorLocalRecord PathIntegrator::evalEmittance(std::shared_ptr<Scene> scene,
                                                        std::optional<Intersection> itsOpt,
                                                        const Ray &ray)
{
    Vec3d wo = -ray.direction;

    Spectrum LEmission(0.0);
    double pdfEmission = 1.0;
    if (!itsOpt.has_value())
    {
        auto record = evalEnvLights(scene, ray);
        LEmission = record.f;
        pdfEmission = record.pdf;
    }
    else if (itsOpt.value().object && itsOpt.value().object->getLight())
    {
        auto its = itsOpt.value();
        Normal3d n = its.geometryNormal;
        auto light = itsOpt.value().object->getLight();
        auto record = light->eval(ray, its, ray.direction);
        LEmission = record.s;
        pdfEmission = record.pdfDirect;
    }
    Spectrum transmittance(1.0); // todo: transmittance eval
    return {ray.direction, transmittance * LEmission, pdfEmission};
}

PathIntegratorLocalRecord PathIntegrator::sampleDirectLighting(std::shared_ptr<Scene> scene,
                                                               const Intersection &its,
                                                               const Ray &ray)
{
    std::shared_ptr<Light> light = chooseOneLight(scene, its, ray, sampler->sample());
    auto record = light->sampleDirect(its, Point2d(sampler->sample(), sampler->sample()), ray.timeMin);
    double pdfEmission = record.pdfEmitDir; // pdfScatter with respect to solid angle
    Vec3d dirScatter = record.wi;
    Spectrum Li = record.s;
    Point3d posL = record.dst;
    Spectrum transmittance(1.0); // todo: visibility test + transmittance eval
    return {dirScatter, Li * transmittance, pdfEmission};
}

PathIntegratorLocalRecord PathIntegrator::evalScatter(std::shared_ptr<Scene> scene,
                                                      const Intersection &its,
                                                      const Ray &ray,
                                                      const Vec3d &dirScatter)
{
    if (its.material != nullptr)
    {
        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
        Normal3d n = its.geometryNormal;
        double wiDotN = dot(n, dirScatter);
        return {
            dirScatter,
            bxdf->f(-ray.direction, dirScatter) * wiDotN,
            bxdf->pdf(-ray.direction, dirScatter)};
    }
    else
    {
        // todo: eval phase function
        return {};
    }
}

PathIntegratorLocalRecord PathIntegrator::sampleScatter(std::shared_ptr<Scene> scene,
                                                        const Intersection &its,
                                                        const Ray &ray)
{
    if (its.material != nullptr)
    {
        Vec3d wo = -ray.direction;
        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
        Vec3d n = its.geometryNormal;
        BxDFSampleResult bsdfSample = bxdf->sample(wo);
        double pdfLastScatterSample = bsdfSample.pdf;
        Vec3d dirScatter = bsdfSample.directionIn;
        double wiDotN = dot(dirScatter, n);
        return {dirScatter, bsdfSample.s * wiDotN, pdfLastScatterSample};
    }
    else
    {
        // todo: sample phase function
        return {};
    }
}

double PathIntegrator::russianRoulette(std::shared_ptr<Scene> scene,
                                       const Intersection &its,
                                       const Spectrum &throughput,
                                       int nBounce)
{
    double pSurvive = std::min(pRussianRoulette, throughput.sum());
    if (nBounce > nPathLengthLimit)
        pSurvive = 0.0;
    return pSurvive;
}

std::shared_ptr<Light> PathIntegrator::chooseOneLight(std::shared_ptr<Scene> scene,
                                                      const Intersection &its,
                                                      const Ray &ray,
                                                      double lightSample)
{
    // uniformly weighted
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    int numLights = lights->size();
    int lightID = std::min(numLights - 1, (int)(lightSample * numLights));
    std::shared_ptr<Light> light = lights->operator[](lightID);
    return light;
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