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

double AbstractPathIntegrator::MISWeight(double x, double y)
{
    double pow_x = std::pow(x, misWeightPower);
    double pow_y = std::pow(y, misWeightPower);
    return pow_x / (pow_x + pow_y);
}

Spectrum AbstractPathIntegrator::Li(const Ray &initialRay, std::shared_ptr<Scene> scene)
{
    Spectrum L(0.0);
    Spectrum throughput(1.0);
    Ray ray = initialRay;
    double pdfLastScatterSample = INFINITY;
    int nBounce = 0;

    while (true)
    {
        std::optional<Intersection> itsOpt = scene->intersect(ray);

        // EVAL EMITTANCE
        PathIntegratorLocalRecord evalLightRecord = evalEmittance(scene, itsOpt, ray);

        if (evalLightRecord.f.isBlack() == false)
        {
            L += throughput * evalLightRecord.f / evalLightRecord.pdf * MISWeight(pdfLastScatterSample, evalLightRecord.pdf);
        }

        // TERMINATE IF NO INTERSECTION
        if (!itsOpt.has_value())
            break;
        Intersection its = itsOpt.value();

        // RUSSIAN ROULETTE
        nBounce++;
        double pSurvive = russianRoulette(scene, its, throughput, nBounce);
        if (randFloat() > pSurvive)
            break;
        throughput /= pSurvive;

        // SAMPLE DIRECT LIGHTING
        // Support multiple direct light samples per intersection. n=1 by default.
        for (int i = 0; i < nDirectLightSamples; i++)
        {
            PathIntegratorLocalRecord sampleLightRecord = sampleDirectLighting(scene, its, ray);
            PathIntegratorLocalRecord evalScatterRecord = evalScatter(scene, its, ray, sampleLightRecord.wi);

            if (sampleLightRecord.f.isBlack() == false)
            {
                L += throughput * sampleLightRecord.f * evalScatterRecord.f / sampleLightRecord.pdf * MISWeight(sampleLightRecord.pdf, evalScatterRecord.pdf) / nDirectLightSamples;
            }
        }

        // SAMPLE SCATTER (BSDF or PHASE)
        PathIntegratorLocalRecord sampleScatterRecord = sampleScatter(scene, its, ray);
        pdfLastScatterSample = sampleScatterRecord.pdf;
        if (sampleScatterRecord.f.isBlack() == false)
        {
            throughput *= sampleScatterRecord.f / sampleScatterRecord.pdf;
        }
        ray = Ray(its.position, sampleScatterRecord.wi);
    }

    return L;
}