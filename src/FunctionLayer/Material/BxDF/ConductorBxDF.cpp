#include "ConductorBxDF.hpp"
#include "CoreLayer/Geometry/Frame.h"
#include "Fresnel.hpp"

ConductorBxDF::ConductorBxDF(Vec3d _eta,Vec3d _k, Spectrum _albedo) :eta(_eta),k(_k),albedo(_albedo){
}


Spectrum ConductorBxDF::f(const Vec3d & out, const Vec3d & in) const {
    return Spectrum();
}

Vec3d ConductorBxDF::sampleWi(const Vec3d & out, const Point2d & sample) const {
    return Frame::reflect(out);
}

double ConductorBxDF::pdf(const Vec3d & out, const Vec3d & in) const {
    return 0;
}

BxDFSampleResult ConductorBxDF::sample(const Vec3d & out, const Point2d & sample) const {
    BxDFSampleResult result;
    result.directionIn = Frame::reflect(out);
    result.pdf = 1;
    Vec3d F = Fresnel::conductorReflectance(eta, k, out.z);
    result.s =  albedo  *  Spectrum(RGB3(F.x,F.y,F.z));
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_SPECULAR);
    return result;
}

bool ConductorBxDF::isSpecular( ) const {
    return false;
}

Spectrum RoughConductorBxDF::f(const Vec3d & out, const Vec3d & in) const {
    return Spectrum();
}

Vec3d RoughConductorBxDF::sampleWi(const Vec3d & out, const Point2d & sample) const {
    return Vec3d();
}

double RoughConductorBxDF::pdf(const Vec3d & out, const Vec3d & in) const {
    return 0;
}

BxDFSampleResult RoughConductorBxDF::sample(const Vec3d & out, const Point2d & sample) const {
    return BxDFSampleResult();
}

bool RoughConductorBxDF::isSpecular( ) const {
    return false;
}
