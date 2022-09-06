#pragma once
#include "Sampler.h"

/// \brief Generate unifromly distributed sample in [0, 1)
/// \ingroup Sampler
class IndependentSampler : public Sampler {
public:
    IndependentSampler() = default;
    
    virtual ~IndependentSampler() = default;

    virtual void startPixel(const Point2i &pixelPosition) override {
        // do nothing       
    }

    virtual void nextSample() override {
        // do nothing
    }

    virtual double sample1D() override {
        return rng();
    }

    virtual Point2d sample2D() override {
        return Point2d{rng(), rng()};
    }
};