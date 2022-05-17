/**
 * @file AbstractPathIntegrator.h
 * @author Zhimin Fan
 * @brief Path Integrator Abstraction
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <cmath>
#include "MonteCarloIntegrator.h"
#include "../Core/FunctionLayer/Ray.h"

struct PathIntegratorLocalRecord
{
    Vec3f wi;
    Spectrum f;
    double pdf;
};

class AbstractPathIntegrator : public MonteCarloIntegrator
{
protected:
public:
    // todo: constructor

    virtual Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene);
    virtual double MISWeight(double x, double y);

    /*************************************************************
    Functions below need to be implemented in derived classes
    *************************************************************/

    // @brief Return emitted radiance along the given ray.
    // @param scene     Ptr to scene.
    // @param its       Intersection that ray hits. If intersection is a emitter,
    //                  it contributes to the returned radiance.
    // @param ray       Current ray, which the returned radiance is along.
    // @return          The field wi is just copied from param ray.
    //                  The field f is the incident radiance at ray.origin,
    //                  in the direction of inverse of ray.direction.
    //                  Note that the probability of importance sampling
    //                  may not be divided, and MIS weight should not be considered.
    //                  The field pdf is the probability of light importance sampling.
    virtual PathIntegratorLocalRecord evalLight(std::shared_ptr<Scene> scene,
                                        const Intersection &its,
                                        const Ray &ray) = 0;

    // @brief Sample incident direction by direct lighting, returning incident radiance at given intersection.
    // @param scene     Ptr to scene.
    // @param its       Intersection that ray hits.
    // @param ray       Current ray, only used to specify out direction.
    // @return          The field wi is the sampled direction.
    //                  The field f is incident radiance at given intersection 
    //                  in the direction of sampled direction wi.
    //                  Note that the probability of importance sampling
    //                  may not be divided, and MIS weight should not be considered.
    //                  The field pdf is the probability of light importance sampling.
    virtual PathIntegratorLocalRecord sampleLight(std::shared_ptr<Scene> scene,
                                          const Intersection &its,
                                          const Ray &ray) = 0;

    // @brief Return scatter value of BSDF or phase function.
    // @param scene     Ptr to scene.
    // @param its       Intersection that ray hits.
    // @param ray       Current ray, only used to specify out direction.
    // @param wi        Incident direction.
    // @return          The field wi is just copied from param wi.
    //                  The field f is the product (of scatter value of BSDF)
    //                  with cosine between shading normal and wi, or phase function.
    //                  The field pdf is the probability of scatter importance sampling.
    virtual PathIntegratorLocalRecord evalScatter(std::shared_ptr<Scene> scene,
                                          const Intersection &its,
                                          const Ray &ray,
                                          const Vec3f &wi) = 0;

    // @brief Sample incident direction by scatter value of BSDF or phase function.
    // @param scene     Ptr to scene.
    // @param its       Intersection that ray hits.
    // @param ray       Current ray, only used to specify out direction.
    // @return          The field wi is the sampled direction.
    //                  The field f is the product (of scatter value of BSDF)
    //                  with cosine between shading normal and wi, or phase function.
    //                  The field pdf is the probability of scatter importance sampling.
    virtual PathIntegratorLocalRecord sampleScatter(std::shared_ptr<Scene> scene,
                                            const Intersection &its,
                                            const Ray &ray) = 0;

    // @brief Return survive probability of Russian roulette.
    virtual double russianRoulette(std::shared_ptr<Scene> scene,
                           const Intersection &its,
                           const Spectrum &T,
                           int nBounce) = 0;
};
