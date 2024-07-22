#pragma once
#include "BxDF.h"
#include "PorousLayerMicrograinBxDF.h"

class PourousLayerBxDF : public BxDF {
public:
    PourousLayerBxDF(std::shared_ptr<MicrograinBxDF> _micrograinBRDF, std::shared_ptr<BxDF> _bulkBxDF);

    Spectrum f(const Vec3d &out, const Vec3d &in) const override;

    double pdf(const Vec3d &out, const Vec3d &in) const override;

    BxDFSampleResult sample(const Vec3d &out, const Point2d &sample) const override;

    // see https://www.shadertoy.com/view/cly3Dt
    double getMicrograinWeight(double _tau0, double _beta, double cosI, double cosO) const {
        cosI = std::clamp(abs(cosI), 0.00001, 1.);
        cosO = std::clamp(abs(cosO), 0.00001, 1.);
        return 1. - ((1. - tauThetaPlus(_tau0, _beta, cosI)) * (1. - tauThetaPlus(_tau0, _beta, cosO)) / (1. - _tau0));
    }
    double gammaBeta(float _beta, float cos) const {
        double beta2 = _beta * _beta;
        double cos2 = cos * cos;
        double sin2 = 1. - cos2;
        return fm::sqrt(beta2 * sin2 + cos2);
    }
    double gammaBetaPlus(float _beta, float cos) const {
        return 0.5 * (cos + gammaBeta(_beta, cos));
    }
    double tauTheta(float _tau0, float _beta, float cos) const {
        return 1. - fm::pow((1. - _tau0), (gammaBeta(_beta, cos) / cos));
    }
    double tauThetaPlus(float _tau0, float _beta, float cos) const {
        return 1. - fm::sqrt((1. - tauTheta(_tau0, _beta, cos)) * (1. - _tau0));
    }

protected:
    std::shared_ptr<MicrograinBxDF> micrograinBRDF;
    std::shared_ptr<BxDF> bulkBxDF;
};