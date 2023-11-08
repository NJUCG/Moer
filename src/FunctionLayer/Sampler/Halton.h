/**
 * @file HaltonSampler.h
 * @author fengzesen (ja50n zs_feng@qq.com)
 * @brief A GlobalSampler using Halton low-discrepancy sequence
 * @version 0.1
 * @date 2022-10-20
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 */
#pragma once
#include "FunctionLayer/Sampler/Sampler.h"
#include "CoreLayer/Math/LowDiscrepancy.h"
// Restrict the resolution for floating precision.
static constexpr int MAX_RESOLUTION = 128;

class HaltonSampler : public GlobalSampler {
public:
    HaltonSampler(const HaltonSampler &) = default;
    HaltonSampler(int64_t _spp, int resolution_w, int resolution_h);
    HaltonSampler(int64_t _spp, int resolution_w, int resolution_h,
                  int _nDim);
    virtual int64_t globalSampleIndex(int64_t pixelSampleIndex) const override;
    virtual double sampleValue(int64_t globalIndex, int dim) const override;
    virtual std::unique_ptr<Sampler> clone(int seed) const override;

private:
    /// @brief All instances share the same perms.
    static std::vector<uint16_t> scramblePermutations;
    /**
     * @brief Value and power required to scale base-2 and base-3
     *        till no less than resoluton_w and resolution_h.
     *        A sample will be discarded if the first two dimensions
     *        exceeded the MAX_RESOLUTION or resolution_[wh] after scaling.
     */
    Point2i scaleVal, scaleExp;
    mutable Point2i pixelForOffset =
        Point2i(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    mutable int64_t globalOffsetForPixelPosition;
    int sampleStride;
    Point2i multInverse;

    /**
     * @brief Beginning of the scramble permutation on dim-th dimension.
     *
     * @param dim Dimension index, starting by zero.
     * @return const uint16_t* Pointer to the first value of the permutation.
     */
    const uint16_t *permutationOfDimension(int dim) const {
        assert(0 <= dim && dim <= nPrimes);
        return &(scramblePermutations[PrimeSums[dim]]);
    }
};