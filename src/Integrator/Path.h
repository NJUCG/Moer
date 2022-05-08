/**
 * @file Path.h
 * @author Zhimin Fan
 * @brief Path Integrator
 * @version 0.1
 * @date 2022-05-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <cmath>
#include "MonteCarloIntegrator.h"
#include "../Core/FunctionLayer/Ray.h"

class PathIntegrator : public MonteCarloIntegrator
{
protected:
public:
    // todo: constructor
    virtual Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene);
};

