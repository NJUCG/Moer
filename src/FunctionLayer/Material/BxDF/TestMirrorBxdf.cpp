#include "TestMirrorBxdf.h"

Spectrum TestMirrorBxdf::f(const Vec3d &wo, const Vec3d &wi) const
{
    return 0.0;
}

double TestMirrorBxdf::pdf(const Vec3d &wo, const Vec3d &wi) const
{
    return 0.0;
}

BxDFSampleResult TestMirrorBxdf::sample(const Vec3d &wo, const Point2d &sample) const
{
    BxDFSampleResult result;

    auto wi = Vec3d (-wo.x,-wo.y,wo.z);
    result.directionIn = wi;
    result.bxdfSampleType= BXDFType(BXDF_SPECULAR | BXDF_REFLECTION);
    result.pdf = 1.0;
    result.s = 1.0;

    return result;
}

