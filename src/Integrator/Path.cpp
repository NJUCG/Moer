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
    double pdfLastScatterSample = INFINITY; // pdfScatter of last BSDF sample
    int nBounce = 0;

    while (true)
    {
        Intersection its = scene->intersect(ray);

        auto [LEmission, pdfEmission] = evalLight(scene, its, ray);
        double misScatter = MISWeight(pdfLastScatterSample, pdfEmission);
        L += throughput * LEmission / pdfEmission * misScatter;

        auto [isAlive, pSurvive] = russianRoulette(scene, its, throughput);
        if (!isAlive)
            break;
        throughput /= pSurvive;
        nBounce++;

        auto [dirLight, LLight, pdfLight] = sampleLight(scene, its, ray);
        double pdfScatterLightSample = evalScatter(scene, its, ray, dirLight);
        double misLight = MISWeight(pdfLight, pdfScatterLightSample);
        L += throughput * LLight / pdfLight * misLight;

        auto [dirScatter, throughputFactor, pdfScatter] = sampleScatter(scene, its, ray);
        pdfLastScatterSample = pdfScatter;
        throughput *= throughputFactor / pdfScatter;
        ray = Ray(its.position, dirScatter);
    }

    return L;
}

std::tuple<Spectrum, double> PathIntegrator::evalLight(std::shared_ptr<Scene> scene,
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
    return {LEmission * transmittance, pdfEmission};
}

std::tuple<Vec3f, Spectrum, double> PathIntegrator::sampleLight(std::shared_ptr<Scene> scene,
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
    // todo: media
    Spectrum bsdf = bxdf->f(-ray.direction, dirScatter);
    double cosine = dot(n, dirScatter);
    Spectrum transmittance(1.0); // todo: visibility test + transmittance eval
    return {dirScatter, Li * bsdf * cosine * transmittance, pdfEmission};
}

double PathIntegrator::evalScatter(std::shared_ptr<Scene> scene,
                                  const Intersection &its,
                                  const Ray &ray,
                                  const Vec3f &dirScatter)
{
    std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
    return bxdf->pdf(-ray.direction, dirScatter);
}

std::tuple<Vec3f, Spectrum, double> PathIntegrator::sampleScatter(std::shared_ptr<Scene> scene,
                                                                  const Intersection &its,
                                                                  const Ray &ray)
{
    Vec3f wo = -ray.direction;
    Vec3f n; // todo: get shading normal
    std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
    // todo: media
    BxDFSampleResult bsdfSample = bxdf->sample(wo);
    double pdfLastScatterSample = bsdfSample.pdf;
    Vec3f dirScatter = bsdfSample.directionIn;
    return {dirScatter, bsdfSample.s * dot(dirScatter, n), pdfLastScatterSample};
}

std::tuple<bool, double> PathIntegrator::russianRoulette(std::shared_ptr<Scene> scene,
                                                         const Intersection &its,
                                                         const Spectrum &throughput)
{
    double pSurvive = std::min(0.95, throughput.sum());
    return {randFloat() < pSurvive && its.object, pSurvive};
}