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
#include "MonteCarlo.h"
#include "../Core/FunctionLayer/Ray.h"

class PathIntegrator : public MonteCarloIntegrator
{
protected:
public:
    // todo: constructor

    virtual Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene);

    // functions below can be override by derived classes, to support path guiding and etc
    std::tuple<Spectrum, double> evalLight(std::shared_ptr<Scene> scene,
                                           const Intersection &its,
                                           const Ray &ray);

    std::tuple<Vec3f, Spectrum, double> sampleLight(std::shared_ptr<Scene> scene,
                                                    const Intersection &its,
                                                    const Ray &ray);

    double evalScatter(std::shared_ptr<Scene> scene,
                       const Intersection &its,
                       const Ray &ray,
                       const Vec3f &wi);

    std::tuple<Vec3f, Spectrum, double> sampleScatter(std::shared_ptr<Scene> scene,
                                                      const Intersection &its,
                                                      const Ray &ray);

    std::tuple<bool, double> russianRoulette(std::shared_ptr<Scene> scene,
                                             const Intersection &its,
                                             const Spectrum &T);
};
