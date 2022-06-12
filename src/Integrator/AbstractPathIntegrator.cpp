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

AbstractPathIntegrator::AbstractPathIntegrator(std::shared_ptr<Camera> camera, std::unique_ptr<Film> film, std::unique_ptr<TileGenerator> tileGenerator, std::shared_ptr<Sampler> sampler, int spp) : MonteCarloIntegrator(camera, std::move(film), std::move(tileGenerator), sampler, spp)
{
}

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
    double pdfLastScatterSample = 1e99;
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
        {
            break;
        }

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
        else
        {
            break;
        }
        const double eps = 1e-4; // todo
        ray = Ray(its.position + sampleScatterRecord.wi * eps, sampleScatterRecord.wi);
    }

    return L;
}