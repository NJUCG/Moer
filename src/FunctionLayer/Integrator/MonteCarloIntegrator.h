/**
 * @file MonteCarloIntegrator.h
 * @author Zhimin Fan
 * @brief Integrators
 * @version 0.1
 * @date 2022-05-06
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <cmath>
#include "Integrator.h"

class MonteCarloIntegrator : public Integrator
{
protected:
    std::shared_ptr<Sampler> sampler;
    int spp = 4;

public:
    MonteCarloIntegrator(std::shared_ptr<Camera> camera, std::unique_ptr<Film> film, std::unique_ptr<TileGenerator> tileGenerator, std::shared_ptr<Sampler> sampler, int spp);

    virtual void render(std::shared_ptr<Scene> scene);

    // @brief Estimate radiance along a given ray
    virtual Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene) = 0;
    // @brief Get a random number WITHOUT using MonteCarloIntegrator::sampler
    virtual double randFloat();
};
