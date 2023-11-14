/**
 * @file VolPathIntegrator.h
 * @author Chenxi Zhou
 * @brief Path Integrator with volume
 * @version 0.1
 * @date 2022-09-22
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#pragma once

#include "CoreLayer/Ray/Ray.h"
#include "AbstractPathIntegrator.h"
#include "FunctionLayer/Medium/Medium.h"

/**
 * @brief Unidirectional path-tracing integrator which
 * take volume into consideration
 * @ingroup Integrator
 */

class VolPathIntegrator : public AbstractPathIntegrator {

public:
    VolPathIntegrator(std::shared_ptr<Camera> _camera,
                      std::unique_ptr<Film> _film,
                      std::unique_ptr<TileGenerator> _tileGenerator,
                      std::shared_ptr<Sampler> _sampler,
                      int _spp,
                      int _renderThreadNum = 4);

    virtual Spectrum Li(const Ray &ray,
                        std::shared_ptr<Scene> scene) override;

    virtual PathIntegratorLocalRecord evalEmittance(std::shared_ptr<Scene> scene,
                                                    std::optional<Intersection> itsOpt,
                                                    const Ray &ray) override;

    virtual PathIntegratorLocalRecord sampleDirectLighting(std::shared_ptr<Scene> scene,
                                                           const Intersection &its,
                                                           const Ray &ray) override;

    virtual PathIntegratorLocalRecord evalScatter(const Intersection &its,
                                                  const Ray &ray,
                                                  const Vec3d &wi) override;

    virtual PathIntegratorLocalRecord sampleScatter(const Intersection &its,
                                                    const Ray &ray) override;

    virtual double russianRoulette(const Spectrum &T,
                                   int nBounce) override;

    virtual std::pair<std::shared_ptr<Light>, double> chooseOneLight(std::shared_ptr<Scene> scene,
                                                                     double lightSample);

    virtual double chooseOneLightPdf(std::shared_ptr<Scene> scene,
                                     std::shared_ptr<Light> light);

    virtual PathIntegratorLocalRecord evalEnvLights(std::shared_ptr<Scene> scene,
                                                    const Ray &ray);

    std::shared_ptr<Medium> getTargetMedium(const Intersection &its,
                                            Vec3d wi) const;

    Spectrum evalTransmittance(std::shared_ptr<Scene> scene,
                               const Intersection &its,
                               Point3d pointOnLight) const;

    Intersection fulfillScatteringPoint(const Point3d &position,
                                        const Normal3d &normal,
                                        std::shared_ptr<Medium> medium);

    std::pair<std::optional<Intersection>, Spectrum>
    intersectIgnoreSurface(std::shared_ptr<Scene> scene,
                           const Ray &ray,
                           std::shared_ptr<Medium> medium) const;

protected:
    const int nPathLengthLimit = 64;
    const double pRussianRoulette = 0.95;
};
