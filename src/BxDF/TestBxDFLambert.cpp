/**
 * @file TestBxDFLambert.cpp
 * @author Zhimin Fan
 * @brief A diffuse BxDF only for unit test of integrator
 * @version 0.1
 * @date 2022-05-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "TestBxDFLambert.h"
#include "../Geometry/CoordConvertor.h"
#include "../Core/FunctionLayer/Intersection.h"

TestBxDFLambert::TestBxDFLambert(Intersection _its) : its(_its)
{
}

Spectrum TestBxDFLambert::f(const Vec3d &out, const Vec3d &in) const
{
    Vec3d n(its.geometryNormal.x, its.geometryNormal.y, its.geometryNormal.z);
    double cosineWi = dot(in, n);
    double cosineWo = dot(out, n);
    if (cosineWi > 0 && cosineWo > 0)
        return diffuse;
    return 0.0;
}

Vec3d TestBxDFLambert::sampleWi(const Vec3d &out, const Point2d &sample) const
{
    // Uniformly sample on the hemisphere (not the disk)
    Vec3d p = CoordConvertor::cartesian2SphericalVec(sample);
    p.z = abs(p.z);
    return CoordConvertor::TBN2World(p, its.geometryTangent, its.geometryBitangent, its.geometryNormal);
}

double TestBxDFLambert::pdf(const Vec3d &out, const Vec3d &in) const
{
    // PDF with respect to solid angle
    Vec3d n(its.geometryNormal.x, its.geometryNormal.y, its.geometryNormal.z);
    double cosineWi = dot(in, n);
    double cosineWo = dot(out, n);
    if (cosineWi > 0 && cosineWo > 0)
        return 1.0 / 2 / 3.14159;
    return 0.0;
}

BxDFSampleResult TestBxDFLambert::sample(const Vec3d &out, const Point2d &sample) const
{
    BxDFSampleResult ans;
    ans.directionIn = sampleWi(out, sample);
    ans.s = f(out, ans.directionIn);
    ans.pdf = pdf(out, ans.directionIn);
    return ans;
}

bool TestBxDFLambert::isSpecular() const
{
    return false;
}
