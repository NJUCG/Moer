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

#include "DielectricBxDF.h"
#include "Fresnel.hpp"
Spectrum DielectricBxDF::f(const Vec3d &wo, const Vec3d &wi) const {
    return {0.f};
}

Vec3d DielectricBxDF::sampleWi(const Vec3d &wo, const Point2d &sample) const {
    double  eta = wo.z < 0.0 ? ior : invIor;
    double cosThetaT;
    double F=Fresnel::DielectricReflectance(eta, std::abs(wo.z), cosThetaT);

    double reflectionProbability= F;
    if(sample[0]<reflectionProbability){
        return Frame::reflect(wo);
    }
    else{
        return  {-eta*wo.x,-eta*wo.y,-std::copysign(cosThetaT,wo.z)};
    }
}

double DielectricBxDF::pdf(const Vec3d &wo, const Vec3d &wi) const {
    return 0;
}

bool DielectricBxDF::isSpecular() const {
    return true ;
}

BxDFSampleResult DielectricBxDF::sample(const Vec3d &wo, const Point2d &sample) const {
    BxDFSampleResult result;

    double  eta = wo.z < 0.0 ? ior : invIor;
    double cosThetaT;
    double F=Fresnel::DielectricReflectance(eta, std::abs(wo.z), cosThetaT);

    double reflectionProbability= F;

    if(sample[0]<=reflectionProbability) {
        result.directionIn = Frame::reflect(wo);
        result.pdf = reflectionProbability;
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_SPECULAR);
        result.s = albedo * F;
    }
    else {
        result.directionIn = Vec3d (-eta*wo.x,-eta*wo.y,-std::copysign(cosThetaT,wo.z));
        // *wi = vec3(0,0,-std::copysign(1,wo.z));
        result.pdf = 1-reflectionProbability;
        result.bxdfSampleType = BXDFType(BXDF_TRANSMISSION | BXDF_SPECULAR);
        result.s= albedo * (1-F);
    }
//    std::cout << "pdf = " <<result.pdf << std::endl;
//    std::cout << "albedo = [" 
//              << albedo.toXYZ3().toRGB3().rgbData[0] << ", "
//              << albedo.toXYZ3().toRGB3().rgbData[1] << ", "
//              << albedo.toXYZ3().toRGB3().rgbData[2] << "]\n"; 
//    std::cout << "s = [" 
//              << result.s.toXYZ3().toRGB3().rgbData[0] << ", "
//              << result.s.toXYZ3().toRGB3().rgbData[1] << ", "
//              << result.s.toXYZ3().toRGB3().rgbData[2] << "]\n"; 
//
    return  result;
}

