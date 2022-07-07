/**
 * @file  Mirror.cpp
 * @author Junping Yuan
 * @brief   Mirror bxdf
 * @version 0.1
 * @date 2022/6/8
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "Mirror.h"


Spectrum Mirror::f(const Vec3d &wo, const Vec3d &wi) const {
   return Spectrum{0.0};
}

Vec3d Mirror::sampleWi(const Vec3d &wo, const Point2d &sample) const {
    return Vec3d (-wo.x,-wo.y,wo.z);
}

double Mirror::pdf(const Vec3d &wo, const Vec3d &wi) const {
    return 0.0;
}

BxDFSampleResult Mirror::sample(const Vec3d &wo, const Point2d &sample) const {
    BxDFSampleResult result;
    result.s=1;
    result.isSpecular= true;
    result.directionIn= sampleWi(wo,sample);
    result.pdf=1;
    return  result;
}

bool Mirror::isSpecular() const {
    return true;
}
