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
#include "CoreLayer/Ray/Ray.h"

struct MediumSampleRecord;

/**
 * @brief 
 * @todo PathIntegratorLocalRecord
 */
struct PathIntegratorLocalRecord {
    Vec3d wi;
    Spectrum f;
    double pdf;
    bool isDelta = false;
};

/**
 * @brief Base class for all path_tracing-like integrators
 * @ingroup Integrator
 */
class AbstractPathIntegrator : public MonteCarloIntegrator {
protected:
    const int nDirectLightSamples = 1;///< Default, sample the direct illumination at each hitpoint once
    const double misWeightPower = 1.0f;

public:
    AbstractPathIntegrator(
        std::shared_ptr<Camera> _camera,
        std::unique_ptr<Film> _film,
        std::unique_ptr<TileGenerator> _tileGenerator,
        std::shared_ptr<Sampler> _sampler,
        int _spp,
        int _renderThreadNum = 4);

    virtual Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene);
    virtual double MISWeight(double x, double y);

    /*************************************************************
    Functions below need to be implemented in derived classes
    *************************************************************/

    /// @brief Return the radiance along given ray, emitted from given intersection.
    /// @param scene     Ptr to scene.
    /// @param its       Intersection hit by ray.
    /// @param ray       Ray to evaluate.
    /// @return          Direction of given ray, incident radiance at origin of ray, pdf of direct light sampling.
    virtual PathIntegratorLocalRecord evalEmittance(std::shared_ptr<Scene> scene,
                                                    std::optional<Intersection> its,
                                                    const Ray &ray) = 0;

    /// @brief Sample incident direction of direct lighting.
    /// @param scene     Ptr to scene.
    /// @param its       Reference point.
    /// @param ray       Ray, used to specify wo (out direction).
    /// @return          Sampled incident direction, incident radiance and pdf per solid angle.
    virtual PathIntegratorLocalRecord sampleDirectLighting(std::shared_ptr<Scene> scene,
                                                           const Intersection &its,
                                                           const Ray &ray) = 0;

    /// @brief Return scatter value of BSDF or phase function.
    /// @param scene     Ptr to scene.
    /// @param its       Reference point.
    /// @param ray       Ray, used to specify wo (out direction).
    /// @param wi        Incident direction wi.
    /// @return          Incident direction, scatter throughput f, pdf per solid angle.
    ///                  For surface, f is the product of BSDF value and cosine term.
    ///                  For medium, f is the value of phase function.
    virtual PathIntegratorLocalRecord evalScatter(const Intersection &its,
                                                  const Ray &ray,
                                                  const Vec3d &wi) = 0;

    /// @brief Sample incident direction by scatter value of BSDF or phase function.
    /// @param scene     Ptr to scene.
    /// @param its       Reference point.
    /// @param ray       Ray, used to specify wo (out direction).
    /// @return          Sampled incident direction, scatter throughput f, pdf per solid angle.
    ///                  For surface, f is the product of BSDF value and cosine term.
    ///                  For medium, f is the value of phase function.
    virtual PathIntegratorLocalRecord sampleScatter(const Intersection &its,
                                                    const Ray &ray) = 0;

    /// @brief Return probability of Russian roulette.
    virtual double russianRoulette(const Spectrum &T,
                                   int nBounce) = 0;
};
