#pragma once

#include "../Core/FunctionLayer/BxDF.h"
#include "../Core/FunctionLayer/Intersection.h"

class TestMirrorBxdf : public BxDF
{
public:
    virtual Spectrum f(const Vec3d &wo, const Vec3d &wi) const;

    virtual Vec3d sampleWi(const Vec3d &wo, const Point2d &sample) const;

    virtual double pdf(const Vec3d &wo, const Vec3d &wi) const;

    virtual BxDFSampleResult sample(const Vec3d &wo, const Point2d &sample) const;

    virtual bool isSpecular() const;
};