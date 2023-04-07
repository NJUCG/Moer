/**
 * @file PathIntegrator.h
 * @author Zhimin Fan
 * @brief Path Integrator
 * @version 0.1
 * @date 2022-05-06
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <cmath>
#include "CoreLayer/Ray/Ray.h"
#include "AbstractPathIntegrator.h"

/**
 * @brief Unidirectional path-tracing integrator
 * @ingroup Integrator
 */
class PathIntegrator : public AbstractPathIntegrator
{
protected:
    const int nPathLengthLimit = 20;        ///< Max path length
    const double pRussianRoulette = 0.95;   ///< The probability to terminate a ray using Russian-Roulette strategy

public:
    PathIntegrator(
        std::shared_ptr<Camera> _camera, 
        std::unique_ptr<Film> _film, 
        std::unique_ptr<TileGenerator> _tileGenerator, 
        std::shared_ptr<Sampler> _sampler, 
        int _spp,
        int _renderThreadNum=4);

    virtual PathIntegratorLocalRecord evalEmittance(std::shared_ptr<Scene> scene,
                                                    std::optional<Intersection> its,
                                                    const Ray &ray) override;

    virtual PathIntegratorLocalRecord sampleDirectLighting(std::shared_ptr<Scene> scene,
                                                           const Intersection &its,
                                                           const Ray &ray) override;

    virtual PathIntegratorLocalRecord evalScatter(const Intersection &its,
                                                  const Ray &ray,
                                                  const Vec3d &wi) override;

    virtual PathIntegratorLocalRecord sampleScatter(const Intersection &its,
                                                    const Ray &ray);

    virtual double russianRoulette(const Spectrum &T,
                                   int nBounce) override;

    virtual std::pair<std::shared_ptr<Light>, double> chooseOneLight(std::shared_ptr<Scene> scene,
                                                                     double lightSample);

    virtual double chooseOneLightPdf(std::shared_ptr<Scene> scene,
                                     std::shared_ptr<Light> light);

    virtual PathIntegratorLocalRecord evalEnvLights(std::shared_ptr<Scene> scene,
                                                    const Ray &ray);
};
