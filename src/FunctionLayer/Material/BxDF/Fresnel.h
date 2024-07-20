/**
 * @file Fresnel.h
 * @author JunPing Yuan
 * @brief compute fresnel items
 * @version 0.1
 * @date 2022-9-6
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include "FastMath.h"
#include "CoreLayer/Geometry/Geometry.h"

namespace Fresnel {

/*
 * The refractive index of dielectric materials is a real number, while the refractive index of conductor materials is a complex number.
 * In order to simplify the computation process, we have created a separate function to calculate the Fresnel term for dielectric materials,
 * while the Fresnel equation for conductor materials is a more general form.
 */

static inline double dielectricReflectance(double eta, double cosThetaI, double &cosThetaT) {
    if (cosThetaI < 0.0) {
        eta = 1.0 / eta;
        cosThetaI = -cosThetaI;
    }
    double sinThetaTSq = eta * eta * (1.0f - cosThetaI * cosThetaI);
    if (sinThetaTSq > 1.0) {
        cosThetaT = 0.0;
        return 1.0;
    }
    cosThetaT = fm::sqrt(std::max(1.0 - sinThetaTSq, 0.0));

    double Rs = (eta * cosThetaI - cosThetaT) / (eta * cosThetaI + cosThetaT);
    double Rp = (eta * cosThetaT - cosThetaI) / (eta * cosThetaT + cosThetaI);

    return (Rs * Rs + Rp * Rp) * 0.5;
}

static inline double dielectricReflectance(double eta, double cosThetaI) {
    double cosThetaT;
    return dielectricReflectance(eta, cosThetaI, cosThetaT);
}

static inline Spectrum dielectricReflectance(Vec3d eta, double cosThetaI) {
    return Spectrum(RGB3{dielectricReflectance(eta[0], cosThetaI), dielectricReflectance(eta[1], cosThetaI), dielectricReflectance(eta[2], cosThetaI)});
}

static inline double conductorReflectance(double eta, double k, double cosThetaI) {
    double cosThetaISq = cosThetaI * cosThetaI;
    double sinThetaISq = std::max(1.0f - cosThetaISq, 0.0);
    double sinThetaIQu = sinThetaISq * sinThetaISq;

    double innerTerm = eta * eta - k * k - sinThetaISq;
    double aSqPlusBSq = fm::sqrt(std::max(innerTerm * innerTerm + 4.0f * eta * eta * k * k, 0.0));
    double a = fm::sqrt(std::max((aSqPlusBSq + innerTerm) * 0.5, 0.0));

    double Rs = ((aSqPlusBSq + cosThetaISq) - (2.0 * a * cosThetaI)) /
                ((aSqPlusBSq + cosThetaISq) + (2.0 * a * cosThetaI));
    double Rp = ((cosThetaISq * aSqPlusBSq + sinThetaIQu) - (2.0f * a * cosThetaI * sinThetaISq)) /
                ((cosThetaISq * aSqPlusBSq + sinThetaIQu) + (2.0f * a * cosThetaI * sinThetaISq));

    return 0.5f * (Rs + Rs * Rp);
}

static inline Spectrum conductorReflectance(const Vec3d &eta, const Vec3d &k, double cosThetaI) {
    return Spectrum(RGB3(
        conductorReflectance(eta.x, k.x, cosThetaI),
        conductorReflectance(eta.y, k.y, cosThetaI),
        conductorReflectance(eta.z, k.z, cosThetaI)));
}

static inline double diffuseReflectance(double eta, double sampleCount) {
    double diffuseFresnel = 0.0;
    double fb = Fresnel::dielectricReflectance(eta, 0.0f);
    for (int i = 1; i <= sampleCount; ++i) {
        double cosThetaSq = double(i) / sampleCount;
        double fa = Fresnel::dielectricReflectance(eta, std::min(std::sqrt(cosThetaSq), 1.0));
        diffuseFresnel += double(fa + fb) * (0.5 / sampleCount);
        fb = fa;
    }

    return double(diffuseFresnel);
}
}// namespace Fresnel
