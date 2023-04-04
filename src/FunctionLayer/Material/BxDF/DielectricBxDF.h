/**
 * @file  Dielectric.h
 * @author Junping Yuan
 * @brief  Dielectric Bxdf
 * @version 0.1
 * @date
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma  once
#include "CoreLayer/Geometry/Frame.h"
#include "BxDF.h"
#include "MicrofacetDistribution.h"

/// \brief Dielectric reflection model
/// \ingroup BxDF
class DielectricBxDF : public  BxDF{
public:

    DielectricBxDF(double _ior, const Spectrum& _specularR,Spectrum _specularT)
        : ior(_ior), invIor(1/ior),specularR(_specularR),specularT(_specularT) {}

    virtual Spectrum f(const Vec3d &wo, const Vec3d &wi) const override ;

    virtual double pdf(const Vec3d &wo, const Vec3d &wi) const override;

    virtual BxDFSampleResult sample(const Vec3d &wo, const Point2d &sample) const override;

    [[nodiscard]]
    double getRoughness() const override { return 0; }

protected:
    double eta(const Vec3d & out,const Vec3d & in) const override;

private:
   double ior;
   double invIor;
   Spectrum specularR,specularT;
};

class RoughDielectricBxDF : public  BxDF{
public:
    RoughDielectricBxDF(double _ior, const Spectrum& _specularR,Spectrum _specularT,
                        double _uRoughness,double _vRoughness,
                        std::shared_ptr<MicrofacetDistribution> _distrib);

    Spectrum f(const Vec3d & out, const Vec3d & in) const override;

    double pdf(const Vec3d & out, const Vec3d & in) const override;

    BxDFSampleResult sample(const Vec3d & out, const Point2d & sample) const override;

    [[nodiscard]]
    double getRoughness() const override { return (alphaXY[0] + alphaXY[1]) / 2.; }

protected:
    double eta(const Vec3d & out, const Vec3d & in) const override;

private:
    double pdf(const Vec3d & out, const Vec3d & in,bool reflected) const ;

    Spectrum f(const Vec3d & out, const Vec3d & in,bool reflected) const ;

    std::shared_ptr<MicrofacetDistribution> distrib;
    Vec2d alphaXY;
    double ior;
    Spectrum glossyR;
    Spectrum glossyT;
};


