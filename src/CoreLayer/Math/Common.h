/**
 * @file  Common.h
 * @author Junping Yuan
 * @brief some common but useful functions
 * @version 0.1
 * @date 2022/06/06
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma  once

#include <cmath>
#include <limits>
#include <utility>
#include <cstdint>
#ifdef _MSC_VER
#include <intrin.h>
#endif

// some constant values
#ifdef _WIN32
constexpr double M_PI       =           3.14159265358979323846f;
#endif
constexpr double INV_PI     =           0.31830988618379067154f;
constexpr double INV_TWOPI  =           0.15915494309189533577f;

constexpr double ONEMINUSEPSILON = 1 - std::numeric_limits<double>::epsilon();
constexpr double EPSILON = std::numeric_limits<double>::epsilon();


template<class T>
inline T clamp(T x, T minVal, T maxVal) {
    if(x>=minVal && x<=maxVal ) return  x;
    return x<minVal?minVal:maxVal;
}

template<class T>
inline T mod(T a, T b) {
    T r = a % b;
    return (r < T(0)) ? r+b : r;
}

template<class T>
T lerp(const T & a,const T & b,double  u){
    return a * (1-u) + b*u;
}

template < class T >
T  lerp(const T & x00, const T & x01, const T & x10, const T & x11, double u, double v
) {

    return (x00*(1.0f - u) + x01*u)*(1.0f - v) +
           (x10*(1.0f - u) + x11*u)*v;
}

template <class T>
inline T sqr(const T & x){
    return x * x;
}


template<class T>
inline T rcp(T x){
    return 1/x;
}

template<class T>
inline T rsqrt(T x){
    return 1/sqrt(x);
}

template<class T>
inline  T select(bool mask,T a,T b){
    return mask?a:b;
}

/// @brief Get the smallest power of 2 non-less than @param v.
inline int32_t pow2Ceil(int32_t v) {
  v--;
  // Make all ones.
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  return v + 1;
}
inline int64_t pow2Ceil(int64_t v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;
  return v + 1;
}
/// @brief Count trailing zeros.
inline int ctz(uint32_t v) { 
#ifdef _MSC_VER
    unsigned long r = 0;
    _BitScanReverse(&r, v);
    return r;
#else
    return __builtin_ctz(v); 
#endif
}