/**
 * @file Stratified.h
 * @author Chenxi Zhou
 * @brief Stratified sampler
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#pragma once

#include "Sampler.h"

/// \brief Stratified generate sample
/// \ingroup Sampler
class StratifiedSampler : public PixelSampler {
    // fill the samples1D
    virtual void generateSamples1D(std::vector<double> &samples) override;
    // fill the samples2D
    virtual void generateSamples2D(std::vector<Point2d> &samples) override;

public:
    StratifiedSampler() = delete;

    StratifiedSampler(int _sppSqrt) : PixelSampler(_sppSqrt * _sppSqrt), sppSqrt(_sppSqrt) {}

    StratifiedSampler(int _sppSqrt, int _nDimensions) : PixelSampler(_sppSqrt * _sppSqrt, _nDimensions), sppSqrt(_sppSqrt) {}

    ~StratifiedSampler() = default;

    std::unique_ptr<Sampler> clone(int seed) const override;
    const int sppSqrt;
};
