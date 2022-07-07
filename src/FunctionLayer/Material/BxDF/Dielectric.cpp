/**
 * @file  Dielectric.cpp
 * @author Junping Yuan
 * @brief  Dielectric Bxdf
 * @version 0.1
 * @date
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "Dielectric.h"

Spectrum Dielectric::f(const Vec3d &wo, const Vec3d &wi) const {
    return {0.f};
}

Vec3d Dielectric::sampleWi(const Vec3d &wo, const Point2d &sample) const {
    float cosThetaI = Frame::cosTheta(wo);

//    return  reflect(wo);
    float fresnelVal = fresnel(cosThetaI,m_extIOR,m_intIOR);
//    if(fresnelVal>sample[0]){
//        return  reflect(wo);
//    }
//    else{
        return  refract(wo);
//    }
}

double Dielectric::pdf(const Vec3d &wo, const Vec3d &wi) const {
    return 0;
}

bool Dielectric::isSpecular() const {
    return true ;
}

BxDFSampleResult Dielectric::sample(const Vec3d &wo, const Point2d &sample) const {
    BxDFSampleResult result;
    result.pdf=1;
    result.directionIn= sampleWi(wo,sample) ;
    result.isSpecular= true;
    result.s=1;
    return  result;
}

