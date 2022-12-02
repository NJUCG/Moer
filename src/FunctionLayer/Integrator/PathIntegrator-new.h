/**
 * @file PathIntegrator-new.h
 * @author Chenxi Zhou
 * @brief Path Integrator with new implementation
 * @version 0.1
 * @date 2022-09-21
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#pragma once

#include "CoreLayer/Ray/Ray.h"
#include "PathIntegratorUtils.h"
#include "AbstractPathIntegrator.h"

/**
 * @brief Unidirectional path-tracing integrator with new 
 * implementation (mitsuba-like)
 * @ingroup Integrator
 */

class PathIntegratorNew : public AbstractPathIntegrator 
{
public:
    PathIntegratorNew (std::shared_ptr<Camera> _camera,
                       std::unique_ptr<Film> _film,
                       std::unique_ptr<TileGenerator> _tileGenerator,
                       std::shared_ptr<Sampler> _sampler,
                       int _spp,
                       int _renderThreadNum = 4);

    virtual Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene) override;

protected:
    const int nPathLengthLimit = 64;
    const double pRussianRoulette = 0.95;
};