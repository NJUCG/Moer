/**
 * @file Warp.h
 * @author Chenxi Zhou
 * @brief The warp function to transform from unifrom sample to specific sample
 * @version 0.1
 * @date 2022-07-18
 * @todo The up direction is (0, 1, 0) or (0, 0, 1) ??
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "Common.h"
#include "CoreLayer/Geometry/Geometry.h"
#include "FastMath.h"

static  double TentInverse(double x){
    if(x<=.5f)
        return fm::sqrt(2*x)-1;
    return  1- fm::sqrt(2-2*x);
}
inline   Point2d SquareToTent(const Point2d &sample) {
    Point2d  res(TentInverse(sample[0]), TentInverse(sample[1]));
    return  res;
}

inline double SquareToTentPdf(const Point2d &p) {
    return (1.0-fm::abs(p[0])) * (1.0-fm::abs(p[1]));
}

inline Vec3d SquareToUniformSphere(const Point2d &sample) {
    float z = 1 - 2 * sample[0];
    float r = fm::sqrt(std::max((float )0, (float)1 - z * z));
    float phi = 2 * M_PI * sample[1];
    return {r * fm::cos(phi), r * fm::sin(phi), z};
}

inline  float SquareToUniformSpherePdf(const Vec3d &v) {
    return 0.25f*INV_PI;
}

inline  Vec3d SquareToUniformHemisphere(const Point2d &sample) {
    float z = 1 - 2 * sample[0];
    float r = fm::sqrt(std::max((float )0, (float)1 - z * z));
    float phi = 2 * M_PI * sample[1];
    return {r * fm::cos(phi), r * fm::sin(phi), fm::abs(z)};
}

inline float SquareToUniformHemispherePdf(const Vec3d &v) {
    return v[2] >=0 ? 0.5f * INV_PI : .0f;
}

inline  Vec3d SquareToCosineHemisphere(const Point2d &sample) {
    float z=sqrt(1-sample.x);
    float phi=sample.y*2*M_PI;

    return {sqrt(sample.x)* cos(phi),sqrt(sample.x)*sin(phi),z};
}

inline  float SquareToCosineHemispherePdf(const Vec3d &v) {
    return v[2] >=0 ? v.z * INV_PI : .0f;
}

inline  Vec3d SquareToBeckmann(const Point2d &sample,double alpha) {
    auto tan2theta= -alpha*alpha*log( sample.x );
    auto cosTheta=sqrt(1/(1+tan2theta));
    auto sinTheta= sqrt(1-cosTheta*cosTheta);
    auto phi=sample.y * 2 * M_PI;
    Vec3d t1= Vec3d(sinTheta*cos(phi), sinTheta*sin(phi),cosTheta);
    return t1;
}

inline  float SquareToBeckmannPdf(const Vec3d &m, double alpha) {
    if(m.z<=0)
        return 0.0f;
    auto cosTheta=m.z;
    auto sinTheta=sqrt(1-cosTheta*cosTheta);
    auto tan2Theta=(sinTheta* sinTheta)/(cosTheta*cosTheta);
    float azimuthal =   INV_PI;
    float longitudinal = exp(-tan2Theta/(alpha*alpha))  / (alpha*alpha*pow(cosTheta,3));
    return azimuthal * longitudinal;
}

// Modified implementation from Mitsuba
inline Point2d SquareToUniformDiskConcentric(const Point2d &sample) {
    double r1 = 2.0 * sample.x - 1.0;
    double r2 = 2.0 * sample.y - 1.0;

    double phi, r;
    if (r1 == 0 && r2 == 0) {
        r = phi = 0;
    } else if (r1 * r1 > r2 * r2) {
        r = r1;
        phi = (M_PI / 4.0) * (r2 / r1);
    } else {
        r = r2;
        phi = (M_PI / 2.0) - (r1 / r2) * (M_PI / 4.0);
    }

    double cosPhi = fm::cos(phi);
    double sinPhi = fm::sin(phi);

    return { r * cosPhi, r * sinPhi };
}
