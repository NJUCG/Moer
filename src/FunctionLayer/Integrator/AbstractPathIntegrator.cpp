/**
 * @file AbstractPathIntegrator.cpp
 * @author Zhimin Fan
 * @brief Path Integrator Abstraction
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "AbstractPathIntegrator.h"
#include "FunctionLayer/Medium/Medium.h"
#include "FastMath.h"

AbstractPathIntegrator::AbstractPathIntegrator(
        std::shared_ptr<Camera> _camera, 
        std::unique_ptr<Film> _film, 
        std::unique_ptr<TileGenerator> _tileGenerator, 
        std::shared_ptr<Sampler> _sampler, 
        int _spp,
        int _renderThreadNum):
            MonteCarloIntegrator(_camera,std::move(_film),std::move(_tileGenerator),_sampler,_spp,_renderThreadNum)
{

}

double AbstractPathIntegrator::MISWeight(double x, double y)
{
    double pow_x = fm::pow(x, misWeightPower);
    double pow_y = fm::pow(y, misWeightPower);
    return pow_x / (pow_x + pow_y);
}

Spectrum AbstractPathIntegrator::Li(const Ray &initialRay, std::shared_ptr<Scene> scene)
{
    Spectrum L(0.0);
    Spectrum throughput(1.0);
    Ray ray = initialRay;
    double pdfLastScatterSample = 1e99;
    bool isLastScatterSampleDelta = false;
    int nBounce = 0;

    while (true)
    {
        std::optional<Intersection> itsOpt = scene->intersect(ray);

        // EVAL EMITTANCE
        PathIntegratorLocalRecord evalLightRecord = evalEmittance(scene, itsOpt, ray);

        if (evalLightRecord.f.isBlack() == false)
        {
            double misw = MISWeight(pdfLastScatterSample, evalLightRecord.pdf);
            if (isLastScatterSampleDelta)
            {
                misw = 1.0;
            }
            L += throughput * evalLightRecord.f * misw;
        }

        // TERMINATE IF NO INTERSECTION
        if (!itsOpt.has_value())
        {
            break;
        }

        Intersection its = itsOpt.value();
        if(ray.hasDifferential){
            its.computeRayDifferential(ray);
        }

        // RUSSIAN ROULETTE
        nBounce++;
        double pSurvive = russianRoulette(throughput, nBounce);
        if (randFloat() > pSurvive)
            break;
        throughput /= pSurvive;

        // SAMPLE DIRECT LIGHTING
        // Support multiple direct light samples per intersection. n=1 by default.
        for (int i = 0; i < nDirectLightSamples; i++)
        {
            PathIntegratorLocalRecord sampleLightRecord = sampleDirectLighting(scene, its, ray);
            PathIntegratorLocalRecord evalScatterRecord = evalScatter(its, ray, sampleLightRecord.wi);

            if (sampleLightRecord.f.isBlack() == false)
            {
                double misw = MISWeight(sampleLightRecord.pdf, evalScatterRecord.pdf);
                if (sampleLightRecord.isDelta)
                {
                    misw = 1.0;
                }
                L += throughput * sampleLightRecord.f * evalScatterRecord.f / sampleLightRecord.pdf * misw / nDirectLightSamples;
            }
        }

        // SAMPLE SCATTER (BSDF or PHASE)
        PathIntegratorLocalRecord sampleScatterRecord = sampleScatter(its, ray);
        pdfLastScatterSample = sampleScatterRecord.pdf;
        isLastScatterSampleDelta = sampleScatterRecord.isDelta;
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
