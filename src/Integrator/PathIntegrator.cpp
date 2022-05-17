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

PathIntegratorLocalRecord PathIntegrator::evalLight(std::shared_ptr<Scene> scene,
                                                    const Intersection &its,
                                                    const Ray &ray)
{
    Vec3f wo = -ray.direction;
    Vec3f n = its.geometryNormal;

    Spectrum LEmission(0.0);
    double pdfEmission = 1.0;
    if (!its.object)
    {
        auto record = evalEnvLights(scene, ray);
        ;
        LEmission = record.f;
        pdfEmission = record.pdf;
    }
    else if (/* hit object is emitter, how to judge? */ 0)
    {
        // todo: get emitted LEmission and pdfEmission of hit object
    }
    Spectrum transmittance(1.0); // todo: transmittance eval
    return {ray.direction, transmittance * LEmission, pdfEmission};
}

PathIntegratorLocalRecord PathIntegrator::sampleLight(std::shared_ptr<Scene> scene,
                                                      const Intersection &its,
                                                      const Ray &ray)
{
    std::shared_ptr<Light> light = chooseOneLight(scene, its, ray, sampler->sample());
    auto record = light->sampleDirect(Point2f(sampler->sample(), sampler->sample()), ray.timeMin);
    double pdfEmission = record.pdfDir; // pdfScatter with respect to solid angle, to our intersection
    Vec3f dirScatter = record.ray.direction;
    Spectrum Li = record.s;
    Point3f posL = record.dst;
    Spectrum transmittance(1.0); // todo: visibility test + transmittance eval
    return {dirScatter, Li * transmittance, pdfEmission};
}

PathIntegratorLocalRecord PathIntegrator::evalScatter(std::shared_ptr<Scene> scene,
                                                      const Intersection &its,
                                                      const Ray &ray,
                                                      const Vec3f &dirScatter)
{
    if (its.material != nullptr)
    {
        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
        Vec3f n = its.geometryNormal;
        double cosine = dot(n, dirScatter);
        return {
            dirScatter,
            bxdf->f(-ray.direction, dirScatter) * cosine,
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
        Vec3f wo = -ray.direction;
        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
        Vec3f n = its.geometryNormal;
        BxDFSampleResult bsdfSample = bxdf->sample(wo);
        double pdfLastScatterSample = bsdfSample.pdf;
        Vec3f dirScatter = bsdfSample.directionIn;
        double cosine = dot(dirScatter, n);
        return {dirScatter, bsdfSample.s * cosine, pdfLastScatterSample};
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
    double pSurvive = std::min(0.95, throughput.sum());
    if (nBounce > 20)
        pSurvive = 0.0;
    return pSurvive;
}

std::shared_ptr<Light> PathIntegrator::chooseOneLight(std::shared_ptr<Scene> scene,
                                                      const Intersection &its,
                                                      const Ray &ray,
                                                      float lightSample)
{
    // may need some prepare process each time we start rendering. where to put it?
    // current implementation uses 3 randnums for light sampling (1 for choosing light, 2 for direct sampling)
    // current implementation uses uniform weight
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    int numLights = lights->size();
    int lightID = std::min(numLights - 1, (int)(lightSample * numLights));
    std::shared_ptr<Light> light = lights->operator[](lightID);
    return light;
}

PathIntegratorLocalRecord PathIntegrator::evalEnvLights(std::shared_ptr<Scene> scene,
                                                        const Ray &ray)
{
    // current implementation just iter through each light and call it's eval(ray)
    // definately, delta lights will return nothing and area lights will not be hitted
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    Spectrum L(0.0);
    float pdf = 0.0;
    for (auto light : *lights)
    {
        auto record = light->eval(ray);
        L += record.s;
        pdf += record.pdfDir;
    }
    return {-ray.direction, L, pdf};
}