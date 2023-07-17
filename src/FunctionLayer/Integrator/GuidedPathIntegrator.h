/**
* @file GuidedPathIntegrator.h
* @author Yiyang Sun
* @brief Path Integrator with path guiding technologies
* @version 0.1
* @date 2023-03-12
*
* @copyright NJUMeta (c) 2023
* www.njumeta.com
*/

#pragma once

#include <atomic>
#include <shared_mutex>
#include "PathIntegrator-new.h"
#include "guiding/guiding.h"

/**
 * @brief Unidirectional path-tracing integrator with path guiding technologies.
 * This Integrator mainly implements the idea as described in
 * Ruppert et al. "Robust fitting of parallax-aware mixtures for path guiding",
 * except that we have omitted the variance-aware split and merge for the present.
 * @ingroup Integrator
 */

class GuidedPathIntegrator : public PathIntegratorNew {
public:

    GuidedPathIntegrator(std::shared_ptr<Camera> _camera,
                      std::unique_ptr<Film> _film,
                      std::unique_ptr<TileGenerator> _tileGenerator,
                      std::shared_ptr<Sampler> _sampler,
                      int _spp,
                      int _renderThreadNum = 4);

    void render(std::shared_ptr<Scene> scene) override;

    /// @brief: render process per thread. Should be called in render().
    void renderPerThread(const std::shared_ptr<Scene> & scene);

    /// @brief Estimate radiance along a given ray
    Spectrum Li(const Ray &initialRay, std::shared_ptr<Scene> scene) override;

    PathIntegratorLocalRecord evalScatter(const Intersection &its, const Ray &ray, const Vec3d &wi) override;

    PathIntegratorLocalRecord sampleScatter(const Intersection &its, const Ray &ray) override;

    double russianRoulette(const Spectrum &T, int nBounce) override;

protected:

    using PGSampleData = PathGuiding::SampleData;
    using AdaptiveKDTree = PathGuiding::kdtree::AdaptiveKDTree;
    using GuidedBxDF = PathGuiding::GuidedBxDF;

    constexpr static double trainingSPPFraction = 0.5;
    constexpr static int sppPerIteration = 4;
    constexpr static int maxSampleBufferSize = 256 * 1024 * 1024 / sizeof(PGSampleData);

    struct BounceInfo {
        Spectrum L{0};
        Spectrum bxdfWeight{0};
        Vec3d position{0};
        Vec3d direction{0};
        double bxdfPdf{0};
        double distance{0};
        double roughness{0};
        double distanceFactor{1};
    };

    std::mutex pgSampleMutex;
    std::vector<PGSampleData> pgSamples;

    std::shared_mutex pgTreeMutex;
    std::shared_ptr<AdaptiveKDTree> pgTree;

    int totalSPP;
    bool isTraining;
    bool isFirstIteration;

    // compute the distance scaling factor required on a refractive surface
    static double computeDistanceFactor(const Intersection & its,
                                        const Vec3d & out,
                                        const Vec3d & in);

    // add a sample to the buffer, optionally triggering the update if
    // the maximum buffer size is exceeded
    void addPGSampleData(const Vec3d & position,
                         const Vec3d & direction,
                         double radiance,
                         double bsdfPdf,
                         double distance);

    // prepare a guided BxDF for sampling and pdf evaluation
    void prepareGuidedBxDF(GuidedBxDF & guidedBxDF,
                           BxDF * bxdf,
                           const Point3d & position);

private:

    // pre-generated tiles used for per-thread rendering
    std::mutex tilesMutex;
    std::vector<std::shared_ptr<Tile>>::iterator tilesBegin;
    std::vector<std::shared_ptr<Tile>>::iterator tilesEnd;

    // the number of samples flushed during the iteration, for logging usage
    size_t numFlashedSamples;

};
