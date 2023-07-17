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

// some constant values
#ifdef _WIN32
constexpr double M_PI       =           3.14159265358979323846f;
#endif
constexpr double INV_PI     =           0.31830988618379067154f;
constexpr double INV_TWOPI  =           0.15915494309189533577f;

constexpr double ONEMINUSEPSILON = 1 - std::numeric_limits<double>::epsilon();
constexpr double EPSILON = std::numeric_limits<double>::epsilon();

/// Simple floating point clamping function
inline float clamp(float value, float min, float max) {
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else return value;
}

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












