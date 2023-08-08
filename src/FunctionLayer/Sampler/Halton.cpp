/**
 * @file Halton.cpp
 * @author fengzesen (ja50n zs_feng@qq.com)
 * @brief Implementions for HaltonSampler.
 * @version 0.1
 * @date 2022-10-21
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 */
#include "Halton.h"

/**
 * @brief Solve x, y for ax + by = 1.
 */
static void exgcd(uint64_t a, uint64_t b, int64_t *x, int64_t *y) {
  if (b == 0) {
    *x = 1;
    *y = 0;
    return;
  }
  int64_t d = a / b, xp, yp;
  exgcd(b, a % b, &xp, &yp);
  *x = xp;
  *y = xp - d * yp;
}
/**
 * @brief Get the multiplicative inverse of a (mod n).
 *        That is, (a * x) % n == 1.
 */
static uint64_t multiplicative_inverse(int64_t a, int64_t n) {
  // a * x = 1 + n * y --> a * x - n * y = 1 --> a * X + b * Y = 1
  int64_t x, y;
  exgcd(a, n, &x, &y);
  return mod(x, n);
}

std::vector<uint16_t> HaltonSampler::scramblePermutations;
HaltonSampler::HaltonSampler(int64_t _sppSqrt, int resolution_w,
                             int resolution_h)
    : HaltonSampler(_sppSqrt, resolution_w, resolution_h, 5) {}
HaltonSampler::HaltonSampler(int64_t _sppSqrt, int resolution_w,
                             int resolution_h, int _nDim)
    : GlobalSampler(_sppSqrt, _nDim) {
  // Generate scramnle perms
  if (scramblePermutations.size() == 0) {
    RandomNumberGenerator rng;
    scramblePermutations = computeScramblePermutations(rng);
  }
  // Compute scales for base-2 and base-3
  Point2i res(resolution_w, resolution_h);
  for (int i = 0; i < 2; i++) {
    int base = i == 0 ? 2 : 3;
    int scale = 1, exp = 0;
    int reso = res[i] < MAX_RESOLUTION ? res[i] : MAX_RESOLUTION;
    while (scale < reso) {
      scale *= base;
      exp++;
    }
    scaleVal[i] = scale;
    scaleExp[i] = exp;
  }
  sampleStride = scaleVal[0] * scaleVal[1];
  // Save the time
  multInverse[0] = multiplicative_inverse(scaleVal[1], scaleVal[0]);
  multInverse[0] = multiplicative_inverse(scaleVal[0], scaleVal[1]);
}

int64_t HaltonSampler::globalSampleIndex(int64_t pixelSampleIndex) const {
  if (pixelPosition != pixelForOffset) {
    // First run after staartPixel() being called.
    globalOffsetForPixelPosition = 0;
    if (sampleStride > 1) {
      Point2i tilePixel(mod(pixelPosition[0], MAX_RESOLUTION),
                        mod(pixelPosition[1], MAX_RESOLUTION));
      for (int i = 0; i < 2; i++) {
        // Get the first index in this pixel
        // for each INDIVIDUAL dimension
        uint64_t dimOffset =
            (i == 0) ? inverseRadicalInverse<2>(tilePixel[i], scaleExp[i])
                     : inverseRadicalInverse<3>(tilePixel[i], scaleExp[i]);
        // Chinese remainder theory,
        // to solve the COMMON index in this pixel
        globalOffsetForPixelPosition +=
            dimOffset * (sampleStride / scaleVal[i]) * multInverse[i];
      }
      globalOffsetForPixelPosition %= sampleStride;
    }
    pixelForOffset = pixelPosition;
  }
  return globalOffsetForPixelPosition + pixelSampleIndex * sampleStride;
}
double HaltonSampler::sampleValue(int64_t globalIndex, int dim) const {
  if (dim == 0) {
    // Base 2 (index 0)
    // Shift is faster
    return radicalInverse(dim, globalIndex >> scaleExp[0]);
  } else if (dim == 1) {
    // Base 3
    return radicalInverse(dim, globalIndex / scaleVal[1]);
  } else {
    // Not position, use scramble
    return radicalInverseScramble(dim, globalIndex,
                                  permutationOfDimension(dim));
  }
}
std::unique_ptr<Sampler> HaltonSampler::clone(int seed) const {
  return std::unique_ptr<Sampler>(new HaltonSampler(*this));
}