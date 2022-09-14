/**
 * @file MonteCarloIntegrator.h
 * @author Zhimin Fan
 * edited by orbitchen at 2022-7-7: apply multithread acceleration and tile generator.
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

/**
 * @brief Base class for all integrators solving rendering equation using MonteCarlo methods
 * @ingroup Integrator
 */
class MonteCarloIntegrator : public Integrator
{
protected:
    std::shared_ptr<Sampler> sampler;
    int spp = 4;                        ///< Default sample per pixel = 4

    std::shared_ptr<TileGenerator> tileGenerator;

    /// @brief: render process per thread. Should be called in render().
    void renderPerThread(std::shared_ptr<Scene> scene);

    int renderThreadNum=4;              ///< Default rendering threads = 4

public:
    MonteCarloIntegrator(
        std::shared_ptr<Camera> _camera, 
        std::unique_ptr<Film> _film, 
        std::unique_ptr<TileGenerator> _tileGenerator, 
        std::shared_ptr<Sampler> _sampler, 
        int _spp,
        int _renderThreadNum=4
        );

    virtual void render(std::shared_ptr<Scene> scene);

    /// @brief Estimate radiance along a given ray
    virtual Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene) = 0;
    
    /// @brief Get a random number WITHOUT using MonteCarloIntegrator::sampler
    virtual double randFloat();
};
