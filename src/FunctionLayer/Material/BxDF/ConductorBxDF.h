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
#include "MicrofacetDistribution.h"

class ConductorBxDF : public  BxDF {
public:

    ConductorBxDF(Vec3d  _eta,Vec3d _k,Spectrum _albedo);

    Spectrum f(const Vec3d & out, const Vec3d & in) const override;

    double pdf(const Vec3d & out, const Vec3d & in) const override;

    BxDFSampleResult sample(const Vec3d & out, const Point2d & sample) const override;

    [[nodiscard]]
    double getRoughness() const override { return 0; }

protected:
    Vec3d  eta;
    Vec3d  k;
    Spectrum albedo;
};

class RoughConductorBxDF : public  BxDF{
public:
    RoughConductorBxDF(Vec3d  _eta,Vec3d _k,Spectrum _albedo, double _uRoughness,double _vRoughness,
                       std::shared_ptr<MicrofacetDistribution> _distrib);

    Spectrum f(const Vec3d & out, const Vec3d & in) const override;

    double pdf(const Vec3d & out, const Vec3d & in) const override;

    BxDFSampleResult sample(const Vec3d & out, const Point2d & sample) const override;

    [[nodiscard]]
    double getRoughness() const override { return (alphaXY[0] + alphaXY[1]) / 2.; }

protected:
    std::shared_ptr<MicrofacetDistribution> distrib;
    Vec2d alphaXY;
    Vec3d eta;
    Vec3d k;
    Spectrum albedo;
};
