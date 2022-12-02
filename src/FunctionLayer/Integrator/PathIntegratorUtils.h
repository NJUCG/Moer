#pragma once

#include "CoreLayer/Ray/Ray.h"
#include "CoreLayer/ColorSpace/Color.h"
#include "FunctionLayer/Scene/Scene.h"
#include "FunctionLayer/Sampler/Sampler.h"
#include <optional>

/**
 * @brief Local Sample record for all path integrator.
 */
struct PathIntegratorLocalRecord {
    Vec3d wi;
    Spectrum f;
    double pdf;
    bool isDelta = false;
};


/// @brief Utilities for all path integrator, including path and volpath.
class PathIntegratorUtils {

public:

	/// @brief Return the radiance along given ray, emitted from given intersection.
    /// @param scene     Ptr to scene.
    /// @param its       Intersection hit by ray.
    /// @param ray       Ray to evaluate.
    /// @return          Direction of given ray (wi), incident radiance at origin of ray (f), pdf of direct light sampling (pdf).
    static PathIntegratorLocalRecord evalEmittance(std::shared_ptr<Scene> scene,
                                                    std::optional<Intersection> itsOpt,
                                                    const Ray &ray);

    /// @brief Sample incident direction of direct lighting.
    /// @param scene     Ptr to scene.
    /// @param its       Reference point.
    /// @param ray       Ray, used to specify wo (out direction).
    /// @return          Sampled incident direction (wi), incident radiance (f) and pdf per solid angle (pdf).
    static PathIntegratorLocalRecord sampleDirectLighting(std::shared_ptr<Scene> scene,
                                                           const Intersection &its,
                                                           const Ray &ray,
                                                           std::shared_ptr<Sampler> sampler);

    /// @brief Return scatter value of BSDF or phase function.
    /// @param scene     Ptr to scene.
    /// @param its       Reference point.
    /// @param ray       Ray, used to specify wo (out direction).
    /// @param wi        Incident direction wi.
    /// @return          Incident direction (wi), scatter throughput (f), pdf per solid angle (pdf).
    ///                  For surface, f is the product of BSDF value and cosine term.
    ///                  For medium, f is the value of phase function.
    static PathIntegratorLocalRecord evalScatter(std::shared_ptr<Scene> scene,
                                                  const Intersection &its,
                                                  const Ray &ray,
                                                  const Vec3d &wi);

    /// @brief Sample incident direction by scatter value of BSDF or phase function.
    /// @param scene     Ptr to scene.
    /// @param its       Reference point.
    /// @param ray       Ray, used to specify wo (out direction).
    /// @return          Sampled incident direction (wi), scatter throughput (f), pdf per solid angle (pdf).
    ///                  For surface, f is the product of BSDF value and cosine term.
    ///                  For medium, f is the value of phase function.
    static PathIntegratorLocalRecord sampleScatter(std::shared_ptr<Scene> scene,
                                                    const Intersection &its,
                                                    const Ray &ray,
                                                    std::shared_ptr<Sampler> sampler);

    /// @brief Return survive probability of Russian roulette.
    static double russianRoulette(int nBounce,
                                    int nPathLengthLimit,
                                    double pRussianRoulette);

    /// @brief Sample a light in scene.
    /// @return Ptr to one light and pdf per light.
    static std::pair<std::shared_ptr<Light>, double> chooseOneLight(std::shared_ptr<Scene> scene,
                                                                     const Intersection &its,
                                                                     const Ray &ray,
                                                                     double lightSample);

    /// @brief Probability of choosing a specified light source.
    static double chooseOneLightPdf(std::shared_ptr<Scene> scene,
                                     const Intersection &its,
                                     const Ray &ray,
                                     std::shared_ptr<Light> light);

    /// @brief Evaluate radiance of env lights.
   static PathIntegratorLocalRecord evalEnvLights(std::shared_ptr<Scene> scene,
                                                    const Ray &ray);
};
