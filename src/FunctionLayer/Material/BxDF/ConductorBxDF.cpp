#include "ConductorBxDF.h"
#include "Fresnel.h"
#include "CoreLayer/Geometry/Frame.h"


ConductorBxDF::ConductorBxDF(Vec3d _eta,Vec3d _k, Spectrum _albedo)
                            :eta(_eta),k(_k),albedo(_albedo){

}


Spectrum ConductorBxDF::f(const Vec3d & out, const Vec3d & in) const {
    return {};
}

double ConductorBxDF::pdf(const Vec3d & out, const Vec3d & in) const {
    return 0;
}

BxDFSampleResult ConductorBxDF::sample(const Vec3d & out, const Point2d & sample) const {
    BxDFSampleResult result;
    if(out.z<0)
        return result;
    result.directionIn = Frame::reflect(out);
    result.pdf = 1;
    auto F = Fresnel::conductorReflectance(eta, k, out.z);
    result.s =  albedo  *  F / (abs(result.directionIn.z));
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_SPECULAR);
    return result;
}

RoughConductorBxDF::RoughConductorBxDF(Vec3d _eta, Vec3d _k, Spectrum _albedo, double _uRoughness, double _vRoughness,
                                       std::shared_ptr < MicrofacetDistribution > _distrib) :
                                        eta(_eta),k(_k),albedo(_albedo),distrib(std::move(_distrib))
                                       {
    alphaXY = Vec2d(distrib->roughnessToAlpha(_uRoughness),distrib->roughnessToAlpha(_vRoughness));
                                       }

Spectrum RoughConductorBxDF::f(const Vec3d & out, const Vec3d & in) const {
    if(out.z<0 || in.z<0)
        return {0.};
    double cosThetaO = AbsCosTheta(out), cosThetaI = AbsCosTheta(in);
    Vec3d wh = in + out;
    // Handle degenerate cases for microfacet reflection
    if (cosThetaI == 0 || cosThetaO == 0) return {0.};
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return {0.};
    wh = normalize(wh);
    double cosI = dot(out,wh.z>0?wh:-wh);
   auto F = Fresnel::conductorReflectance(eta,k,cosI);
    return albedo * (distrib->D(wh,alphaXY)  * F * distrib->G(out, in,alphaXY)/ (4 * cosThetaI * cosThetaO));
}

double RoughConductorBxDF::pdf(const Vec3d & out, const Vec3d & in) const {
    if ( CosTheta(out) < 0 || CosTheta(in) < 0) return 0;
    Vec3d wh = normalize(out + in);
    return distrib->Pdf(out, wh,alphaXY) / (4 * dot(out, wh));
}

BxDFSampleResult RoughConductorBxDF::sample(const Vec3d & out, const Point2d & sample) const {
    BxDFSampleResult result{};
    if(CosTheta(out)<0){
        return result;
    }
    Vec3d wh = distrib->Sample_wh(out,sample,alphaXY);
    Vec3d in = Frame::reflect(out,wh);
    if (dot(wh,out) < 0 || CosTheta(in)<0)
    {
        result.s=Spectrum(0);
        return result;
    }
    double mPdf =distrib->Pdf(out,wh,alphaXY);
    auto woDoth = dot(out,wh);
    double pdf = mPdf*0.25f/woDoth;
    Spectrum F = Fresnel::conductorReflectance(eta, k, woDoth);
    result.pdf = pdf;
    result.s = ( F * distrib->G(out,in,alphaXY)*distrib->D(wh,alphaXY))/abs(4 * out.z * in.z);
    result.directionIn = in;
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    return result;
}

