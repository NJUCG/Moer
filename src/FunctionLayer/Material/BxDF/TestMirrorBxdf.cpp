#include "TestMirrorBxdf.h"

Spectrum TestMirrorBxdf::f(const Vec3d &wo, const Vec3d &wi) const
{
    return 0.0;
}

Vec3d TestMirrorBxdf::sampleWi(const Vec3d &wo, const Point2d &sample) const
{
    Vec3d wi = -wo;
    wi.z = -wi.z;
    return wi;
}

double TestMirrorBxdf::pdf(const Vec3d &wo, const Vec3d &wi) const
{
    return 0.0;
}

BxDFSampleResult TestMirrorBxdf::sample(const Vec3d &wo, const Point2d &sample) const
{
    BxDFSampleResult result;

    auto wi = sampleWi(wo, sample);
    result.directionIn = wi;
    result.isSpecular = false;
    result.pdf = 1.0;
    result.s = 1.0;

    return result;
}

bool TestMirrorBxdf::isSpecular() const
{
    return false;
}
