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
        // todo: get environment LEmission and pdfEmission
    }
    else if (/* hit object is emitter */ 0)
    {
        // todo: get emitted LEmission and pdfEmission
    }
    Spectrum transmittance(1.0); // todo: transmittance eval
    return {ray.direction, transmittance * LEmission, pdfEmission};
}

PathIntegratorLocalRecord PathIntegrator::sampleLight(std::shared_ptr<Scene> scene,
                                                      const Intersection &its,
                                                      const Ray &ray)
{
    Vec3f wo = -ray.direction;
    Vec3f n = its.geometryNormal;
    std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
    // todo: sample all lights and fill these vars below
    double pdfEmission; // pdfScatter with respect to solid angle, to our intersection
    Vec3f dirScatter;
    Spectrum Li;
    Point3f posL;
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