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
//constexpr double M_PI       =           3.14159265358979323846f;    ///< \todo multiple defination M_PI
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

/// Simple integer clamping function
inline int clamp(int value, int min, int max) {
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else return value;
}


/// Always-positive modulo operation
inline int mod(int a, int b) {
    int r = a % b;
    return (r < 0) ? r+b : r;
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











