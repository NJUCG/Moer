/**
 * @file AbstractPathIntegrator.h
 * @author Zhimin Fan
 * @brief Path Integrator Abstraction
 * edit by zcx 2022-9-24
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "MonteCarloIntegrator.h"
#include "PathIntegratorUtils.h"
#include "CoreLayer/Ray/Ray.h"

/**
 * @brief Base class for all path_tracing-like integrators
 * @ingroup Integrator
 */
class AbstractPathIntegrator : public MonteCarloIntegrator
{
protected:
    const int nDirectLightSamples = 1;      ///< Default, sample the direct illumination at each hitpoint once
    const double misWeightPower = 1.0f;

public:
    AbstractPathIntegrator(
        std::shared_ptr<Camera> _camera, 
        std::unique_ptr<Film> _film, 
        std::unique_ptr<TileGenerator> _tileGenerator, 
        std::shared_ptr<Sampler> _sampler, 
        int _spp,
        int _renderThreadNum=4);

    virtual Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene);
    virtual double MISWeight(double x, double y);

};
