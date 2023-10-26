/**
 * @file LowDiscrepancy.h
 * @author fengzesen (ja50n zs_feng@qq.com)
 * @brief Utility or math functions about low-discrepancy sequences.
 * @version 0.1
 * @date 2022-10-20
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 */
#pragma once
#include <stdint.h>
#include <vector>
#include "Common.h"
#include "CoreLayer/Adapter/Random.h"
#include "CoreLayer/Geometry/Geometry.h"
static constexpr int nPrimes = 1000;
extern const int Primes[nPrimes];
extern const int PrimeSums[nPrimes];

template<typename T>
void shuffle(T *array, int nSample, int nDim, RandomNumberGenerator &rng) {
    for (int i = 0; i < nSample; ++i) {
        int other = i + rng(0, nSample - i);
        for (int j = 0; j < nDim; ++j) {
            std::swap(array[nDim * i + j], array[nDim * other + j]);
        }
    }
}
/**
 * @brief Generate the permutation used to scramble the Hammersley and Halton
 *        sequence.
 *
 * @param rng                     Random number generator.
 * @return std::vector<uint16_t>  Computed permutations, in a continuous memory.
 */
std::vector<uint16_t> computeScramblePermutations(RandomNumberGenerator &rng);
/**
 * @brief Represent value a under the base of baseIndex-th prime,
 *        then inverse the representation to right of the decimal point.
 *        e.g. 42 is 1120 under base 3 (the 1-st prime), the inversed is
 *             0.0211 under base 3, so radicalInverse(1, 3) returns 0.2716...
 *
 * @param baseIndex Index of primes, used as the base.
 * @param a         Value to be inversed.
 * @return double   The inversed value.
 */
double radicalInverse(int baseIndex, uint64_t a);
/**
 * @brief In order to avoid the regular patterns when base gets large,
 *        we use a random permutation to scramble the inverse digits.
 *
 * @param baseIndex Index of primes, used as the base.
 * @param a         Value to be inversed.
 * @param perm      Permutation used to scramble the digits.
 * @return double   The inversed and scrambled value.
 */
double radicalInverseScramble(int baseIndex, uint64_t a, const uint16_t *perm);

inline uint32_t reverseBits32(uint32_t x) {
    x = (x << 16) | (x >> 16);
    x = ((x & 0x00ff00ff) << 8) | ((x & 0xff00ff00) >> 8);
    x = ((x & 0x0f0f0f0f) << 4) | ((x & 0xf0f0f0f0) >> 4);
    x = ((x & 0x33333333) << 2) | ((x & 0xcccccccc) >> 2);
    x = ((x & 0x55555555) << 1) | ((x & 0xaaaaaaaa) >> 1);
    return x;
}
inline uint64_t reverseBits64(uint64_t x) {
    uint64_t high32 = reverseBits32(uint32_t(x));
    uint64_t low32 = reverseBits32(uint32_t(x >> 32));
    return (high32 << 32) | low32;
}
/**
 * @brief Take the inversed digits and get the original value back.
 *        Inverse transform of radicalInverse().
 *        e.g. The inversed digits of 42 based 3 is 22(10), 4 digits,
 *             then the inverseRadicalInverse<3>(22, 4) should return 42
 * @tparam base
 * @param inverse   Inversed digits.
 * @param nDigits   To maintain the trailing zeros.
 * @return uint64_t Just the value.
 */
template<int base>
inline uint64_t inverseRadicalInverse(uint64_t inverse, int nDigits) {
    uint64_t index = 0;
    for (int i = 0; i < nDigits; ++i) {
        uint64_t digit = inverse % base;
        inverse /= base;
        index = index * base + digit;
    }
    return index;
}

/// @brief Fill a series of sample values by generator matrix.
/// @param C The first column of 32x32 generator matrix.
/// @param n Number of sample values to be filled.
/// @param scramble_bits Encoded scramble bits.
/// @param p Store the sample values.
inline void fillSampleByGraycode(const uint32_t *C, uint32_t n,
                                 uint32_t scrambleBits, double *p) {
    uint32_t v = scrambleBits;// Fisrt value.
    for (uint32_t i = 0; i < n; ++i) {
        p[i] = v * 0x1p-32f;
        // Next value.
        // The changed bit position of Gray code is equal to
        // # of trailing zeros.
        // i.e. from 011-th to 100-th code, which is 010 to 110,
        // the changed bit index is 2, # trailing zeros of 100.
        v ^= C[ctz(i + 1)];
    }
}
/// @brief 2D version.
inline void fillSampleByGraycode(const uint32_t *C0, const uint32_t *C1,
                                 uint32_t n, const Point2i &scrambleBits,
                                 Point2d *p) {
    uint32_t v[2] = {(uint32_t)scrambleBits.x, (uint32_t)scrambleBits.y};
    for (uint32_t i = 0; i < n; ++i) {
        p[i].x = std::min(v[0] * 0x1p-32, ONEMINUSEPSILON);
        p[i].y = std::min(v[1] * 0x1p-32, ONEMINUSEPSILON);
        v[0] ^= C0[ctz(i + 1)];
        v[1] ^= C1[ctz(i + 1)];
    }
}

inline void fillVDCorput1D(int nSubSample, int spp,
                           double *sampleValues, RandomNumberGenerator &rng) {
    uint32_t scrambleBits = rng();
    // Generator matrix. Identity.
    const uint32_t CVanDerCorput[32] = {
        // clang-format off
        0x80000000, 0x40000000, 0x20000000, 0x10000000,
        0x8000000,  0x4000000,  0x2000000,  0x1000000,
        0x800000,   0x400000,   0x200000,   0x100000,
        0x80000,    0x40000,    0x20000,    0x10000,
        0x8000,     0x4000,     0x2000,     0x1000,
        0x800,      0x400,      0x200,      0x100,
        0x80,       0x40,       0x20,       0x10,
        0x8,        0x4,        0x2,        0x1
        // clang-format on
    };
    int nTotalSample = nSubSample * spp;
    fillSampleByGraycode(CVanDerCorput, nTotalSample, scrambleBits,
                         sampleValues);
    // Shuffle to cancel the correlation.
    for (int i = 0; i < spp; ++i)
        shuffle(sampleValues + i * nSubSample, nSubSample, 1, rng);
    shuffle(sampleValues, spp, nSubSample, rng);
}
inline void fillSobol2D(int nSubSample, int spp,
                        Point2d *sampleValues, RandomNumberGenerator &rng) {
    Point2i scramble;
    scramble[0] = rng();
    scramble[1] = rng();
    // Two matrices.
    const uint32_t CSobol[2][32] = {
        // clang-format off
        {
            0x80000000, 0x40000000, 0x20000000, 0x10000000, 
            0x8000000,  0x4000000,  0x2000000,  0x1000000,
            0x800000,   0x400000,   0x200000,   0x100000,
            0x80000,    0x40000,    0x20000,    0x10000,
            0x8000,     0x4000,     0x2000,     0x1000,
            0x800,      0x400,      0x200,      0x100,
            0x80,       0x40,       0x20,       0x10,
            0x8,        0x4,        0x2,        0x1
        },
        {
            0x80000000, 0xc0000000, 0xa0000000, 0xf0000000,
            0x88000000, 0xcc000000, 0xaa000000, 0xff000000, 
            0x80800000, 0xc0c00000, 0xa0a00000, 0xf0f00000,
            0x88880000, 0xcccc0000, 0xaaaa0000, 0xffff0000, 
            0x80008000, 0xc000c000, 0xa000a000, 0xf000f000,
            0x88008800, 0xcc00cc00, 0xaa00aa00, 0xff00ff00,
            0x80808080, 0xc0c0c0c0, 0xa0a0a0a0, 0xf0f0f0f0, 
            0x88888888, 0xcccccccc, 0xaaaaaaaa, 0xffffffff
      }
        // clang-format on
    };
    fillSampleByGraycode(CSobol[0], CSobol[1], nSubSample * spp,
                         scramble, sampleValues);
    // Shuffle to cancel the correlation.
    for (int i = 0; i < spp; ++i)
        shuffle(sampleValues + i * nSubSample, nSubSample, 1, rng);
    shuffle(sampleValues, spp, nSubSample, rng);
}