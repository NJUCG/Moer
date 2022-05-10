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

Spectrum PathIntegrator::Li(const Ray &ray0, std::shared_ptr<Scene> scene)
{
    Spectrum L(0.0);
    Spectrum T(1.0); // throughput

    Ray ray = ray0;
    double pdfB = INFINITY; // pdf of last BSDF sample
    int nBounce = 0;

    while (true)
    {
        Intersection its = scene->intersect(ray);

        auto [Le, pdfL] = evalLight(scene, its, ray);
        double mis = MISWeight(pdfB, pdfL);
        L += T * Le / pdfL * mis;

        auto [b, q] = russianRoulette(scene, its, T);
        if (!b)
            break;
        nBounce++;

        auto [wl, Ll, pdfL1] = sampleLight(scene, its, ray);
        double pdfB1 = evalScatter(scene, its, ray, wl);
        double mis2 = MISWeight(pdfL, pdfB1);
        L += T * Ll / pdfL1 * mis2;

        auto [wi, t, pdf] = sampleScatter(scene, its, ray);
        pdfB = pdf;
        T *= t / pdf;
        ray = Ray(its.position, wi);
    }

    return L;
}

std::tuple<Spectrum, double> PathIntegrator::evalLight(std::shared_ptr<Scene> scene,
                                                      const Intersection &its,
                                                      const Ray &ray)
{
    Vec3f wo = -ray.direction;
    Vec3f n; // todo: get shading normal

    Spectrum Le(0.0);
    double pdfL = 1.0;
    if (!its.object)
    {
        // todo: get environment Le and pdfL
    }
    else if (/* hit object is emitter */ 0)
    {
        // todo: get emitted Le and pdfL
    }
    Spectrum transmittance(1.0); // todo: transmittance eval
    return {Le * transmittance, pdfL};
}

std::tuple<Vec3f, Spectrum, double> PathIntegrator::sampleLight(std::shared_ptr<Scene> scene,
                                                                const Intersection &its,
                                                                const Ray &ray)
{
    Vec3f wo = -ray.direction;
    Vec3f n; // todo: get shading normal
    std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
    // todo: sample all lights and fill these vars below
    double pdfL; // pdf with respect to solid angle, to our intersection
    Vec3f wi;
    Spectrum Li;
    Point3f posL;
    // todo: media
    Spectrum bsdf = bxdf->f(-ray.direction, wi);
    double cosine = dot(n, wi);
    Spectrum transmittance(1.0); // todo: visibility test + transmittance eval
    return {wi, Li * bsdf * cosine * transmittance, pdfL};
}

double PathIntegrator::evalScatter(std::shared_ptr<Scene> scene,
                                  const Intersection &its,
                                  const Ray &ray,
                                  const Vec3f &wi)
{
    std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
    return bxdf->pdf(-ray.direction, wi);
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
    double pdfB = bsdfSample.pdf;
    Vec3f wi = bsdfSample.directionIn;
    return {wi, bsdfSample.s * dot(wi, n), pdfB};
}

std::tuple<bool, double> PathIntegrator::russianRoulette(std::shared_ptr<Scene> scene,
                                                         const Intersection &its,
                                                         const Spectrum &T)
{
    double q = std::min(0.95, T.sum());
    return {randFloat() < q && its.object, q};
}