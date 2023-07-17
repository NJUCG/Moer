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
#include "Fresnel.h"
#include "FastMath.h"
Spectrum DielectricBxDF::f(const Vec3d &wo, const Vec3d &wi) const {
    return {0.f};
}

double DielectricBxDF::pdf(const Vec3d &wo, const Vec3d &wi) const {
    return 0;
}

BxDFSampleResult DielectricBxDF::sample(const Vec3d &wo, const Point2d &sample) const {
    BxDFSampleResult result;

    double  eta = wo.z < 0.0 ? ior : invIor;
    double cosThetaT;
    double F= Fresnel::dielectricReflectance(eta, fm::abs(wo.z), cosThetaT);
    double reflectionProbability= F;
    if(sample[0]<=reflectionProbability) {
        result.directionIn = Frame::reflect(wo);
        result.pdf = reflectionProbability;
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_SPECULAR);
        result.s = specularR * F / fm::abs(result.directionIn.z);
    }
    else {
        result.directionIn = Vec3d (-eta*wo.x,-eta*wo.y,-std::copysign(cosThetaT,wo.z));
        result.pdf = 1-reflectionProbability;
        result.bxdfSampleType = BXDFType(BXDF_TRANSMISSION | BXDF_SPECULAR);
        result.s= specularT * (1-F)/ fm::abs(result.directionIn.z);
    }
    return  result;
}

double DielectricBxDF::eta(const Vec3d & out,const Vec3d & in) const {
    if(out.z * in.z>0)
        return 1;
    return out.z>0?1/ior:ior;
}


RoughDielectricBxDF::RoughDielectricBxDF(double _ior, const Spectrum & _specularR, Spectrum _specularT,
                                         double _uRoughness, double _vRoughness,
                                         std::shared_ptr < MicrofacetDistribution > _distrib)
                                         : ior(_ior), glossyR(_specularR), glossyT(_specularT), distrib(_distrib){
    alphaXY = Vec2d(distrib->roughnessToAlpha(_uRoughness),distrib->roughnessToAlpha(_vRoughness));
}

Spectrum RoughDielectricBxDF::f(const Vec3d & out, const Vec3d & in) const {
    bool reflect = out.z * in.z > 0;
    return f(out,in,reflect);
}


double RoughDielectricBxDF::pdf(const Vec3d & out, const Vec3d & in) const {
    bool reflect = out.z * in.z > 0;
    return pdf(out,in,reflect);
}

BxDFSampleResult RoughDielectricBxDF::sample(const Vec3d & out, const Point2d & sample) const {
    BxDFSampleResult result;
    Vec3d wh = distrib->Sample_wh(out,sample,alphaXY);

    double whDotOut = dot(out, wh);
    double  cosThetaT;
    double  F = Fresnel::dielectricReflectance(1/ior,whDotOut,cosThetaT);
    double  r = rand() % (10000 + 1) / (float)(10000 + 1);
    bool reflect = r<F;
    if(reflect){
        Vec3d in = -out + 2 * dot(out, wh) * wh;
        result.directionIn = in;
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    }
    else {
        double eta = whDotOut < 0.0f ? ior : 1.0f/ior;
        Vec3d in = (eta * whDotOut - (whDotOut>0?1:-1)*cosThetaT)* wh - eta* out ;
        result.directionIn = in;
        result.bxdfSampleType = BXDFType(BXDF_TRANSMISSION | BXDF_GLOSSY);
    }
    result.s = f(out,result.directionIn,reflect) ;
    result.pdf = pdf(out,result.directionIn,reflect);
    return result;
}

double RoughDielectricBxDF::pdf(const Vec3d & out, const Vec3d & in, bool reflect) const {
    double eta = out.z >0 ? 1/ior : ior;
    Vec3d wh;
    double  pdf;
    if(reflect){
        wh = normalize(in+out) * std::copysign(1,out.z);
    }
    else {
        wh = -normalize(in + out * eta);
    }
    double  F =  Fresnel::dielectricReflectance(1/ior,dot(out,wh));
    double whPdf = distrib->Pdf(out,wh,alphaXY);
    if(whPdf < 1e-50)
    {
        return 0;
    }
    if(reflect){
        pdf = F * whPdf / (4 * absDot(out,wh));
    }
    else {
        double sqrtDenom = dot(out, wh) * eta +  dot(in, wh);
        double dWhDWi =
                fm::abs( dot(in, wh)) / (sqrtDenom * sqrtDenom);
        pdf =   whPdf * (1-F) * dWhDWi;
    }
    return pdf;
}

Spectrum RoughDielectricBxDF::f(const Vec3d & out, const Vec3d & in, bool reflect) const {
    double eta = out.z >0 ? 1/ior : ior;
    Vec3d  wh;
    if (reflect) {
        wh = std::copysign(1,out.z) * normalize(in + out);
    } else {
        wh = -normalize(in + out * eta);
    }
    double F = Fresnel::dielectricReflectance(1/ior,dot(out,wh));
    double D = distrib->D(wh,alphaXY);
    double G = distrib->G(in,out,alphaXY);
    if (reflect) {
        return glossyR * F * D * G /  (4 * abs(in.z * out.z));
    }
    else {
        double whDotIn =  dot(wh,in);
        double whDotOut = dot(wh,out);
        double sqrtDeom = eta * whDotOut  +  whDotIn;
        return glossyT * ( 1 - F) * D * G * fm::abs(
                whDotIn * whDotOut  /
                (in.z * out.z * sqrtDeom * sqrtDeom)
        );
    }
}

double RoughDielectricBxDF::eta(const Vec3d & out, const Vec3d & in) const {
    if(out.z * in.z>0)
        return 1;
    return out.z>0?1/ior:ior;
}


