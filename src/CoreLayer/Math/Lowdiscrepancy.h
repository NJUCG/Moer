/**
 * @file Lowdiscrepancy.h
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
static constexpr int nPrimes = 1000;
extern const int Primes[nPrimes];
extern const int PrimeSums[nPrimes];

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
template <int base>
inline uint64_t inverseRadicalInverse(uint64_t inverse, int nDigits) {
    uint64_t index = 0;
    for (int i = 0; i < nDigits; ++i) {
        uint64_t digit = inverse % base;
        inverse /= base;
        index = index * base + digit;
    }
    return index;
}
