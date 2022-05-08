/**
 * @file MonteCarloIntegrator.h
 * @author Zhimin Fan
 * @brief Integrators
 * @version 0.1
 * @date 2022-05-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <cmath>
#include "../Core/FunctionLayer/Integrator.h"

class MonteCarloIntegrator : public Integrator
{
protected:
    std::shared_ptr<Sampler> sampler;

public:
    // todo: constructor

    virtual void render(std::shared_ptr<Scene> scene);

    // @brief Estimate radiance on a given ray
    virtual Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene) = 0;
    virtual double MISWeight(double x, double y);
    // @brief Get random without using MonteCarloIntegrator::sampler
    virtual double randFloat();
};

