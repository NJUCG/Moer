#include "ConductorBxDF.h"
#include "Fresnel.h"
#include "CoreLayer/Geometry/Frame.h"


ConductorBxDF::ConductorBxDF(Vec3d _eta,Vec3d _k, Spectrum _albedo)
                            :eta(_eta),k(_k),albedo(_albedo){

}


Spectrum ConductorBxDF::f(const Vec3d & out, const Vec3d & in) const {
    return Spectrum();
}

double ConductorBxDF::pdf(const Vec3d & out, const Vec3d & in) const {
    return 0;
}

BxDFSampleResult ConductorBxDF::sample(const Vec3d & out, const Point2d & sample) const {
    BxDFSampleResult result;
    result.directionIn = Frame::reflect(out);
    result.pdf = 1;
    Vec3d F = Fresnel::conductorReflectance(eta, k, out.z);
    result.s =  albedo  *  Spectrum(RGB3(F.x,F.y,F.z)) / (abs(result.directionIn.z));
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_SPECULAR);
    return result;
}

RoughConductorBxDF::RoughConductorBxDF(Vec3d _eta, Vec3d _k, Spectrum _albedo, double _uRoughness, double _vRoughness,
                                       std::shared_ptr < MicrofacetDistribution > _distrib) :
                                        eta(_eta),k(_k),albedo(_albedo),distrib(_distrib)
                                       {
    alphaXY = Vec2d(distrib->roughnessToAlpha(_uRoughness),distrib->roughnessToAlpha(_vRoughness));
                                       }

Spectrum RoughConductorBxDF::f(const Vec3d & out, const Vec3d & in) const {


    double cosThetaO = AbsCosTheta(out), cosThetaI = AbsCosTheta(in);
    Vec3d wh = in + out;
    // Handle degenerate cases for microfacet reflection
    if (cosThetaI == 0 || cosThetaO == 0) return Spectrum(0.);
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return Spectrum(0.);
    wh = normalize(wh);
    // For the Fresnel call, make sure that wh is in the same hemisphere
    // as the surface normal, so that TIR is handled correctly.
    double cosI = dot(in,wh.z>0?wh:-wh);
    Vec3d F = Fresnel::conductorReflectance(eta,k,cosI);
    Vec3d weight = (distrib->D(wh,alphaXY)  * F * distrib->G(out, in,alphaXY)/ (4 * cosThetaI * cosThetaO));
    RGB3 rgbSpectrum = albedo.toRGB3() * RGB3(weight.x,weight.y,weight.z);
    return Spectrum(rgbSpectrum);
}

double RoughConductorBxDF::pdf(const Vec3d & out, const Vec3d & in) const {
    if ( CosTheta(out) * CosTheta(in) < 0) return 0;
    Vec3d wh = normalize(out + in);
    return distrib->Pdf(out, wh,alphaXY) / (4 * dot(out, wh));
}

BxDFSampleResult RoughConductorBxDF::sample(const Vec3d & out, const Point2d & sample) const {
    BxDFSampleResult result;
    Vec3d wh = distrib->Sample_wh(out,sample,alphaXY);
    Vec3d in = Frame::reflect(out,wh);
    if (CosTheta(out) * CosTheta(in) < 0)
    {
        result.s=Spectrum(0);
        return result;
    }
    result.s = f(out,in);
    result.directionIn = in;
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    result.pdf =distrib->Pdf(out, wh,alphaXY) / (4 * dot(out, wh));
    return result;
}

