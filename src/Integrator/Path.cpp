/**
 * @file Path.cpp
 * @author Zhimin Fan
 * @brief Path Integrator
 * @version 0.1
 * @date 2022-05-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Path.h"

Spectrum PathIntegrator::Li(const Ray &initialRay, std::shared_ptr<Scene> scene)
{
    Spectrum L(0.0);
    Spectrum throughput(1.0);
    Ray ray = initialRay;
    double pdfLastScatterSample = INFINITY;
    int nBounce = 0;

    while (true)
    {
        Intersection its = scene->intersect(ray);
        PathIntegratorLocalRecord evalLightRecord = evalLight(scene, its, ray);
        L += throughput * evalLightRecord.f / evalLightRecord.pdf * MISWeight(pdfLastScatterSample, evalLightRecord.pdf);

        nBounce++;
        double pSurvive = russianRoulette(scene, its, throughput, nBounce);
        if (randFloat() > pSurvive)
            break;
        throughput /= pSurvive;

        PathIntegratorLocalRecord sampleLightRecord = sampleLight(scene, its, ray);
        PathIntegratorLocalRecord evalScatterRecord = evalScatter(scene, its, ray, sampleLightRecord.wi);
        L += throughput * sampleLightRecord.f * evalScatterRecord.f / sampleLightRecord.pdf * MISWeight(sampleLightRecord.pdf, evalScatterRecord.pdf);

        PathIntegratorLocalRecord sampleScatterRecord = sampleScatter(scene, its, ray);
        pdfLastScatterSample = sampleScatterRecord.pdf;
        throughput *= sampleScatterRecord.f / sampleScatterRecord.pdf;
        ray = Ray(its.position, sampleScatterRecord.wi);
    }

    return L;
}

PathIntegratorLocalRecord PathIntegrator::evalLight(std::shared_ptr<Scene> scene,
                                                    const Intersection &its,
                                                    const Ray &ray)
{
    Vec3f wo = -ray.direction;
    Vec3f n; // todo: get shading normal

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
    Vec3f n; // todo: get shading normal
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
    std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
    // todo: media scatter
    Vec3f n; // todo: get shading normal
    double cosine = dot(n, dirScatter);
    return {
        dirScatter,
        bxdf->f(-ray.direction, dirScatter) * cosine,
        bxdf->pdf(-ray.direction, dirScatter)};
}

PathIntegratorLocalRecord PathIntegrator::sampleScatter(std::shared_ptr<Scene> scene,
                                                        const Intersection &its,
                                                        const Ray &ray)
{
    Vec3f wo = -ray.direction;
    std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
    // todo: media scatter
    Vec3f n; // todo: get shading normal
    BxDFSampleResult bsdfSample = bxdf->sample(wo);
    double pdfLastScatterSample = bsdfSample.pdf;
    Vec3f dirScatter = bsdfSample.directionIn;
    double cosine = dot(dirScatter, n);
    return {dirScatter, bsdfSample.s * cosine, pdfLastScatterSample};
}

double PathIntegrator::russianRoulette(std::shared_ptr<Scene> scene,
                                       const Intersection &its,
                                       const Spectrum &throughput,
                                       int nBounce)
{
    // todo: define these const params as member data
    double pSurvive = std::min(0.95, throughput.sum());
    if (nBounce > 20)
        pSurvive = 0.0;
    return pSurvive;
}