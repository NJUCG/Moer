/**
 * @file TestBxDFLambert.h
 * @author Zhimin Fan
 * @brief A diffuse BxDF only for unit test of integrator
 * @version 0.1
 * @date 2022-05-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../Core/FunctionLayer/BxDF.h"
#include "../Core/FunctionLayer/Intersection.h"

class TestBxDFLambert : public BxDF
{
    Intersection its;

public:
    TestBxDFLambert(Intersection its);
    virtual Spectrum f(const Vec3d &out, const Vec3d &in) const;
    virtual Vec3d sampleWi(const Vec3d &out, const Point2d &sample) const;
    virtual double pdf(const Vec3d &out, const Vec3d &in) const;
    virtual BxDFSampleResult sample(const Vec3d &out, const Point2d &sample) const;
    virtual bool isSpecular() const;

    Spectrum diffuse;
};