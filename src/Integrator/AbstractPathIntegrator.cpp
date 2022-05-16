/**
 * @file AbstractPathIntegrator.cpp
 * @author Zhimin Fan
 * @brief Path Integrator Abstraction
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "AbstractPathIntegrator.h"

Spectrum AbstractPathIntegrator::Li(const Ray &initialRay, std::shared_ptr<Scene> scene)
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