#include "PorousLayerBxDF.h"

PourousLayerBxDF::PourousLayerBxDF(std::shared_ptr<MicrograinBxDF> _micrograinBRDF, std::shared_ptr<BxDF> _bulkBxDF)
    : micrograinBRDF(_micrograinBRDF), bulkBxDF(_bulkBxDF) {
}
// TODO: a better importance sampling
Spectrum PourousLayerBxDF::f(const Vec3d &out, const Vec3d &in) const {
    double tau0 = micrograinBRDF->GetTau0();
    double beta = micrograinBRDF->GetBeta();

    double w = getMicrograinWeight(tau0, beta, CosTheta(in), CosTheta(out));
    auto fs = w * micrograinBRDF->f(out, in);
    return w * micrograinBRDF->f(out, in) + (1. - w) * bulkBxDF->f(out, in);
}

double PourousLayerBxDF::pdf(const Vec3d &out, const Vec3d &in) const {
    double tau0 = micrograinBRDF->GetTau0();
    double beta = micrograinBRDF->GetBeta();

    // double w = getMicrograinWeight(tau0, beta, CosTheta(in), CosTheta(out));*/
    double sampleWeight = tau0;
    return sampleWeight * micrograinBRDF->pdf(out, in) + (1. - sampleWeight) * bulkBxDF->pdf(out, in);

    return CosTheta(in) / fm::pi_d;
}

BxDFSampleResult PourousLayerBxDF::sample(const Vec3d &out, const Point2d &sample) const {
    double tau0 = micrograinBRDF->GetTau0();
    double beta = micrograinBRDF->GetBeta();

    bool chooseBulkLobe = false;
    ////double w = getMicrograinWeight(tau0, beta, CosTheta(in), CosTheta(out));
    BxDFSampleResult result;
    double sampleWeight = tau0;
    if (sample[0] < sampleWeight) {
        double resample = sample[0] / sampleWeight;
        result = micrograinBRDF->sample(out, {resample, sample[1]});
    } else {
        result = bulkBxDF->sample(out, sample);
        chooseBulkLobe = true;
    }
    double w = getMicrograinWeight(tau0, beta, CosTheta(result.directionIn), CosTheta(out));
    if (chooseBulkLobe) {
        result.s = (1 - w) * result.s + w*micrograinBRDF->f(out, result.directionIn);
    } else {
        result.s = w * result.s + (1 - w) * bulkBxDF->f(out, result.directionIn);
    }
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    result.pdf = pdf(out, result.directionIn);
    return result;

    /*double theta = fm::acos(fm::sqrt(1 - sample[0]));
    double phi = 2 * fm::pi_d * sample[1];
    Vec3d in = {fm::cos(phi) * fm::sin(theta), fm::sin(phi) * fm::sin(theta), fm::cos(theta)};

    BxDFSampleResult result;
    result.pdf = pdf(out, in);
    result.s = f(out, in);
    result.bxdfSampleType = BXDFType(BXDF_GLOSSY | BXDF_REFLECTION);
    result.directionIn = in;
    return result;*/
}
