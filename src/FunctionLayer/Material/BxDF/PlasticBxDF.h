#include "BxDF.h"
#include <memory>

class MicrofacetDistribution;

class Plastic : public BxDF {
public:
    double pdf(const Vec3d &out, const Vec3d &in) const override;


    Plastic(const Spectrum &diffuseR, double ior, double diffuseFresnel, double avgTransmittance, const Spectrum &scaledSigmaA);

    [[nodiscard]]
    double getRoughness() const override;

private:
    BxDFSampleResult sample(const Vec3d &out, const Point2d &sample) const override;

    Spectrum f(const Vec3d &out, const Vec3d &in) const override;

    Spectrum diffuseR;
    double ior;
    double _diffuseFresnel;
    double _avgTransmittance;
    Spectrum _scaledSigmaA;
};

class RoughPlastic : public BxDF {
public:
    double pdf(const Vec3d &out, const Vec3d &in) const override;

    BxDFSampleResult sample(const Vec3d &out, const Point2d &sample) const override;

    Spectrum f(const Vec3d &out, const Vec3d &in) const override;

    [[nodiscard]]
    double getRoughness() const override;

//    RoughPlastic(const Spectrum &_glossyR, const Spectrum &_diffuseR, double _ior, double _uRoughness, double _vRoughness,
//                 const std::shared_ptr<MicrofacetDistribution> &_distrib);
    RoughPlastic(const Spectrum &diffuseR, double ior, double diffuseFresnel, double avgTransmittance, const Spectrum &scaledSigmaA,double _uRoughness, double _vRoughness,
            const std::shared_ptr<MicrofacetDistribution> &_distrib);

private:
    Spectrum diffuseR;
    double _diffuseFresnel;
    double _avgTransmittance;
    Spectrum _scaledSigmaA;
    double ior;
    std::shared_ptr<MicrofacetDistribution> distrib;
    Vec2d alphaXY;
};
