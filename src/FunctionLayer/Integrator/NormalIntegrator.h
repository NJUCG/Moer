/**
 * @file NormalIntegrator.h
 * @author JunPing Yuan
 * @brief Show Normal For Debug Purpose
 * @version 0.1
 * @date 2022-09-27
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "MonteCarloIntegrator.h"

class NormalIntegrator : public MonteCarloIntegrator {
public:
    NormalIntegrator(std::shared_ptr<Camera> _camera,
                     std::unique_ptr<Film> _film,
                     std::unique_ptr<TileGenerator> _tileGenerator,
                     std::shared_ptr<Sampler> _sampler,
                     int _spp,
                     int _renderThreadNum) : MonteCarloIntegrator(_camera, std::move(_film), std::move(_tileGenerator), _sampler, _spp, _renderThreadNum) {}

    Spectrum Li(const Ray &ray, std::shared_ptr<Scene> scene) override {
        auto its = scene->intersect(ray);
        if (its.has_value()) {
            Normal3d normal = its->shFrame.n;
            auto res = RGB3(normal.x, normal.y, normal.z) + RGB3(1);
            return res / 2;
        }
        return Spectrum(0);
    }
};