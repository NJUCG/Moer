#pragma once
#include "BxDF.h"
#include "MicrofacetDistribution.h"

class MicrograinBxDF : public BxDF {
public:
    MicrograinBxDF(double _tau0, double _beta) : tau0(_tau0), beta(_beta), distrib(std::make_shared<MicrograinDistribution>()) {
    }
    double GetTau0() const { return tau0; }
    double GetBeta() const { return beta; }
    Spectrum f(const Vec3d &out, const Vec3d &in) const override = 0;

protected:
    double tau0;
    double beta;
    std::shared_ptr<MicrograinDistribution> distrib;
};
class ConductorMicrograinBxDF : public MicrograinBxDF {
public:
    ConductorMicrograinBxDF(Spectrum _R0, Vec3d _k, double _tau0, double _beta);

    Spectrum f(const Vec3d &out, const Vec3d &in) const override;

    double pdf(const Vec3d &out, const Vec3d &in) const override;

    BxDFSampleResult sample(const Vec3d &out, const Point2d &sample) const override;

protected:
    Spectrum R0;
    Vec3d k;
};

class PlasticMicrograinBxDF : public MicrograinBxDF {
public:
    PlasticMicrograinBxDF(Spectrum _R0, Spectrum _kd, double _tau0, double _beta);

    Spectrum f(const Vec3d &out, const Vec3d &in) const override;

    double pdf(const Vec3d &out, const Vec3d &in) const override;

    BxDFSampleResult sample(const Vec3d &out, const Point2d &sample) const override;

protected:
    Spectrum R0;
    Spectrum kd;
};
