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
#define M_PI         3.14159265358979323846f    ///< \todo multiple defination M_PI
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f

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

/// Linearly interpolate between two values
inline float lerp(float t, float v1, float v2) {
    return ((float) 1 - t) * v1 + t * v2;
}

/// Always-positive modulo operation
inline int mod(int a, int b) {
    int r = a % b;
    return (r < 0) ? r+b : r;
}











