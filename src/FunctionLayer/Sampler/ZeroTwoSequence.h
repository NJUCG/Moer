/**
 * @file ZeroTwoSequence.h
 * @author fengzesen (ja50n zs_feng@qq.com)
 * @brief PixelSampler using (0, 2)-sequence.
 *
 * @version 0.1
 * @date 2023-08-10
 * @copyright NJUMeta (c) 2023
 * www.njumeta.com
 */
#pragma once
#include "FunctionLayer/Sampler/Sampler.h"
// #include "CoreLayer/Math/LowDiscrepancy.h"
#include "CoreLayer/Math/Common.h"
class ZeroTwoSampler : public PixelSampler {
protected:
    void generateSamples1D(std::vector<double> &samples) override;
    void generateSamples2D(std::vector<Point2d> &samples) override;

public:
    ZeroTwoSampler(int64_t _spp, int _nDim) : PixelSampler(round(_spp), _nDim) {
    }

    std::unique_ptr<Sampler> clone(int seed) const override {
        ZeroTwoSampler *sampler = new ZeroTwoSampler(*this);
        return std::unique_ptr<Sampler>(sampler);
    }
    int round(int n) override { return pow2Ceil(n); }
};
