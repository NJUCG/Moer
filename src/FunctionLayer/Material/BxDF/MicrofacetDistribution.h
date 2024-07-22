/**
 * @file MicrofacetDistribution.h
 * @author JunPing.Yuan
 * @brief some MicrofactDistirbution models
 * @version 0.1
 * @date 2022-10-5
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */
#pragma once

#include "CoreLayer/Adapter/JsonUtil.h"

class MicrofacetDistribution {
public:
    // MicrofacetDistribution Public Methods
    virtual ~MicrofacetDistribution();
    // https://www.pbr-book.org/3ed-2018/Reflection_Models/Microfacet_Models
    virtual double roughnessToAlpha(double roughness) const = 0;
    virtual double D(const Vec3d &wh, const Vec2d &alphaXY) const = 0;
    virtual double Lambda(const Vec3d &w, const Vec2d &alphaXY) const = 0;
    double G1(const Vec3d &w, const Vec2d &alphaXY) const {
        //    if (Dot(w, wh) * CosTheta(w) < 0.) return 0.;
        return 1 / (1 + Lambda(w, alphaXY));
    }
    virtual double G(const Vec3d &wo, const Vec3d &wi, const Vec2d &alphaXY) const {
        return G1(wo, alphaXY) * G1(wi, alphaXY);
        return 1 / (1 + Lambda(wo, alphaXY) + Lambda(wi, alphaXY));
    }
    virtual Vec3d Sample_wh(const Vec3d &wo, Point2d u, const Vec2d &alphaXY) const = 0;
    double Pdf(const Vec3d &wo, const Vec3d &wh, const Vec2d &alphaXY) const;
    virtual std::string ToString() const = 0;
    inline bool sampleVisible() {
        return sampleVisibleArea;
    }

protected:
    // MicrofacetDistribution Protected Methods
    MicrofacetDistribution(bool sampleVisibleArea)
        : sampleVisibleArea(sampleVisibleArea) {
    }

    // MicrofacetDistribution Protected Data
    const bool sampleVisibleArea;
};

class BeckmannDistribution : public MicrofacetDistribution {
public:
    // For Beckmann sample visible normals is not supported yet.
    BeckmannDistribution(bool sampleVis = false) : MicrofacetDistribution(false) {}
    double roughnessToAlpha(double roughness) const override;
    double D(const Vec3d &wh, const Vec2d &alphaXY) const override;
    Vec3d Sample_wh(const Vec3d &wo, Point2d u, const Vec2d &alphaXY) const override;
    std::string ToString() const override;

protected:
    double Lambda(const Vec3d &w, const Vec2d &alphaXY) const override;
};

class GGXDistribution : public MicrofacetDistribution {
public:
    GGXDistribution(bool sampleVis = false) : MicrofacetDistribution(sampleVis) {}
    double roughnessToAlpha(double roughness) const override;

    double D(const Vec3d &wh, const Vec2d &alphaXY) const override;

    double G(const Vec3d &wo, const Vec3d &wi, const Vec2d &alphaXY) const override;

    double Lambda(const Vec3d &w, const Vec2d &alphaXY) const override;

    Vec3d Sample_wh(const Vec3d &wo, Point2d u, const Vec2d &alphaXY) const override;

    std::string ToString() const override;
};

class MicrograinDistribution : public MicrofacetDistribution {
public:
    MicrograinDistribution(bool sampleVis = false) : MicrofacetDistribution(sampleVis) {}
    double roughnessToAlpha(double roughness) const override {
        assert(0 && "you should not use this function when using Micrograin Ditribution");
        return 0;
    }

    double D(const Vec3d &wh, const Vec2d &alphaXY) const override;

    Vec3d Sample_wh(const Vec3d &wo, Point2d u, const Vec2d &alphaXY) const override;

    std::string ToString() const override;

protected:
    double Lambda(const Vec3d &w, const Vec2d &alphaXY) const override;
};

std::shared_ptr<MicrofacetDistribution> LoadDistributionFromJson(const Json &json);

// namespace  Mirofacet {
//     class Distribution{
//
//     };
// }
