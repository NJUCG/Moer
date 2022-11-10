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

    StratifiedSampler(int sppSqrt) : PixelSampler(sppSqrt) { }

    StratifiedSampler(int sppSqrt, int _nDimensions) : PixelSampler(sppSqrt, _nDimensions) { }

    ~StratifiedSampler() = default;
    
};
