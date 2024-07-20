#include "PorousLayerBxDF.h"

PourousLayerBxDF::PourousLayerBxDF(std::shared_ptr<MicrograinBxDF> _micrograinBRDF, std::shared_ptr<BxDF> _bulkBxDF)
    : micrograinBRDF(_micrograinBRDF), bulkBxDF(_bulkBxDF) {
}
//TODO: a better importance sampling
Spectrum PourousLayerBxDF::f(const Vec3d &out, const Vec3d &in) const {
    double tau0 = micrograinBRDF->GetTau0();
    double beta = micrograinBRDF->GetBeta();
    
    // double w = getMicrograinWeight(tau0, beta, CosTheta(in), CosTheta(out));
    double w = 0.5;
    return micrograinBRDF->f(out, in) + (1. - w) * bulkBxDF->f(out, in);
}

double PourousLayerBxDF::pdf(const Vec3d &out, const Vec3d &in) const {
    double tau0 = micrograinBRDF->GetTau0();
    double beta = micrograinBRDF->GetBeta();

    // double w = getMicrograinWeight(tau0, beta, CosTheta(in), CosTheta(out));
    double w = 0.5;
    return micrograinBRDF->pdf(out, in) + (1. - w) * bulkBxDF->pdf(out, in);
}

BxDFSampleResult PourousLayerBxDF::sample(const Vec3d &out, const Point2d &sample) const {
    double tau0 = micrograinBRDF->GetTau0();
    double beta = micrograinBRDF->GetBeta();

    // double w = getMicrograinWeight(tau0, beta, CosTheta(in), CosTheta(out));
    BxDFSampleResult result;
    double w = 0.5;
    if (rand() * 1.0 / RAND_MAX < w) {
        result = micrograinBRDF->sample(out, sample);
    } else {
        result = bulkBxDF->sample(out, sample);
    }
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    result.s = f(out, result.directionIn);
    result.pdf = pdf(out, result.directionIn);
    return result;
}
