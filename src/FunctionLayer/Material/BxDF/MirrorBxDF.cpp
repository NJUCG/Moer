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

#include "MirrorBxDF.h"


Spectrum MirrorBxDF::f(const Vec3d &wo, const Vec3d &wi) const {
   return Spectrum{0.0};
}

Vec3d MirrorBxDF::sampleWi(const Vec3d &wo, const Point2d &sample) const {
    return Vec3d (-wo.x,-wo.y,wo.z);
}

double MirrorBxDF::pdf(const Vec3d &wo, const Vec3d &wi) const {
    return 0.0;
}

BxDFSampleResult MirrorBxDF::sample(const Vec3d &wo, const Point2d &sample) const {
    BxDFSampleResult result;
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_SPECULAR);
    result.directionIn= sampleWi(wo,sample);
    result.pdf=1;
    result.s = 1 / std::abs(result.directionIn.z);
    return  result;
}

bool MirrorBxDF::isSpecular() const {
    return true;
}
