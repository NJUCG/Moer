/**
 * @file
 * @author Junping Yuan 
 * @brief 
 * @version 0.1
 * @date 
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma  once
#include "Diffuse.h"


Diffuse::Diffuse(Spectrum albedo) :albedo(albedo) {
}


Spectrum Diffuse::f(const Vec3d & wo , const Vec3d & wi) const {
    if(Frame::cosTheta(wi )<0 || Frame::cosTheta(wo )<0){
        return 0;
    }
    return albedo * INV_PI ;

}

Vec3d Diffuse::sampleWi(const Vec3d & wo , const Point2d &sample) const {
    return SquareToUniformHemisphere(sample);
}

double Diffuse::pdf(const Vec3d &wo , const Vec3d & wi) const {
    return SquareToUniformHemispherePdf(wi);
}

BxDFSampleResult Diffuse::sample(const Vec3d  & wo, const Point2d &sample) const {\
    BxDFSampleResult result ;

    auto wi = sampleWi(wo,sample);
    result.directionIn = wi ;
    result.isSpecular = false;
    result.pdf = pdf(wo,wi);
    result.s = f(wo,wi);

    return result;
}

bool Diffuse::isSpecular() const {
    return false;
}

