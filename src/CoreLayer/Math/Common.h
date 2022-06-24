/**
 * @file  Common.h
 * @author Junping Yuan
 * @brief some  common but useful functions
 * @version 0.1
 * @date 2022/06/06
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma  once
#include <math.h>
#include <limits>
// some constant values
#define M_PI         3.14159265358979323846f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f

constexpr double ONEMINUSEPSILON = 1 - std::numeric_limits<double>::epsilon();

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

inline  float fresnel(float cosThetaI, float extIOR, float intIOR) {
    float etaI = extIOR, etaT = intIOR;

    if (extIOR == intIOR)
        return 0.0f;

    /* Swap the indices of refraction if the interaction starts
       at the inside of the object */
    if (cosThetaI < 0.0f) {
        std::swap(etaI, etaT);
        cosThetaI = -cosThetaI;
    }

    /* Using Snell's law, calculate the squared sine of the
       angle between the normal and the transmitted ray */
    float eta = etaI / etaT,
            sinThetaTSqr = eta*eta * (1-cosThetaI*cosThetaI);

    if (sinThetaTSqr > 1.0f)
        return 1.0f;  /* Total internal reflection! */

    float cosThetaT = std::sqrt(1.0f - sinThetaTSqr);

    float Rs = (etaI * cosThetaI - etaT * cosThetaT)
               / (etaI * cosThetaI + etaT * cosThetaT);
    float Rp = (etaT * cosThetaI - etaI * cosThetaT)
               / (etaT * cosThetaI + etaI * cosThetaT);

    return (Rs * Rs + Rp * Rp) / 2.0f;
}

inline  double fresnel(double cosThetaI, double extIOR, double intIOR) {
    double etaI = extIOR, etaT = intIOR;

    if (extIOR == intIOR)
        return 0.0;

    /* Swap the indices of refraction if the interaction starts
       at the inside of the object */
    if (cosThetaI < 0.0f) {
        std::swap(etaI, etaT);
        cosThetaI = -cosThetaI;
    }

    /* Using Snell's law, calculate the squared sine of the
       angle between the normal and the transmitted ray */
    double eta = etaI / etaT,
            sinThetaTSqr = eta*eta * (1-cosThetaI*cosThetaI);

    if (sinThetaTSqr > 1.0f)
        return 1.0f;  /* Total internal reflection! */

    double cosThetaT = std::sqrt(1.0f - sinThetaTSqr);

    double Rs = (etaI * cosThetaI - etaT * cosThetaT)
               / (etaI * cosThetaI + etaT * cosThetaT);
    double Rp = (etaT * cosThetaI - etaI * cosThetaT)
               / (etaT * cosThetaI + etaI * cosThetaT);

    return (Rs * Rs + Rp * Rp) / 2.0;
}







