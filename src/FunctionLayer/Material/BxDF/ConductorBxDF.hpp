/**
 * @file ConductorBxDF.h
 * @author JunPing Yuan
 * @brief Conductor BxDF
 * @version 0.1
 * @date 2022-9-6
 *
 * @copyright Copyright (c) 2022
 *
 */


#pragma once
#include "BxDF.h"

class ConductorBxDF : public  BxDF {
public:
    ConductorBxDF(Vec3d  _eta,Vec3d _k,Spectrum _albedo);

    Spectrum f(const Vec3d & out, const Vec3d & in) const override;

    Vec3d sampleWi(const Vec3d & out, const Point2d & sample) const override;

    double pdf(const Vec3d & out, const Vec3d & in) const override;

    BxDFSampleResult sample(const Vec3d & out, const Point2d & sample) const override;

    bool isSpecular( ) const override;

protected:
    Vec3d  eta;
    Vec3d  k;
    Spectrum albedo;
};

class RoughConductorBxDF : public  BxDF{
public:
    Spectrum f(const Vec3d & out, const Vec3d & in) const override;

    Vec3d sampleWi(const Vec3d & out, const Point2d & sample) const override;

    double pdf(const Vec3d & out, const Vec3d & in) const override;

    BxDFSampleResult sample(const Vec3d & out, const Point2d & sample) const override;

    bool isSpecular( ) const override;

protected:
    double roughness;
    Vec3d  eta;
    Vec3d  k;
    Spectrum albedo;
};
