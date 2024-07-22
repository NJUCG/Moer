#include "PorousLayerBxDF.h"

PourousLayerBxDF::PourousLayerBxDF(std::shared_ptr<MicrograinBxDF> _micrograinBRDF, std::shared_ptr<BxDF> _bulkBxDF)
    : micrograinBRDF(_micrograinBRDF), bulkBxDF(_bulkBxDF) {
}
// TODO: a better importance sampling
Spectrum PourousLayerBxDF::f(const Vec3d &out, const Vec3d &in) const {
    double tau0 = micrograinBRDF->GetTau0();
    double beta = micrograinBRDF->GetBeta();

    double w = getMicrograinWeight(tau0, beta, CosTheta(in), CosTheta(out));
    return w * micrograinBRDF->f(out, in) + (1. - w) * bulkBxDF->f(out, in);
}

double PourousLayerBxDF::pdf(const Vec3d &out, const Vec3d &in) const {
    /*double tau0 = micrograinBRDF->GetTau0();
    double beta = micrograinBRDF->GetBeta();*/

    // double w = getMicrograinWeight(tau0, beta, CosTheta(in), CosTheta(out));*/
    /*double w = tau0;
    return w * micrograinBRDF->pdf(out, in) + (1. - w) * bulkBxDF->pdf(out, in);*/

    return CosTheta(in) / fm::pi_d;
}

BxDFSampleResult PourousLayerBxDF::sample(const Vec3d &out, const Point2d &sample) const {
    // double tau0 = micrograinBRDF->GetTau0();
    // double beta = micrograinBRDF->GetBeta();

    ////double w = getMicrograinWeight(tau0, beta, CosTheta(in), CosTheta(out));
    // BxDFSampleResult result;
    // double w = tau0;
    // if (rand() * 1.0 / RAND_MAX < w) {
    //     result = micrograinBRDF->sample(out, sample);
    // } else {
    //     result = bulkBxDF->sample(out, sample);

    //}
    // result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    // result.s = f(out, result.directionIn);
    // result.pdf = pdf(out, result.directionIn);
    // return result;

    double theta = fm::acos(fm::sqrt(1 - sample[0]));
    double phi = 2 * fm::pi_d * sample[1];
    Vec3d in = {fm::cos(phi) * fm::sin(theta), fm::sin(phi) * fm::sin(theta), fm::cos(theta)};

    BxDFSampleResult result;
    result.pdf = pdf(out, in);
    result.s = f(out, in);
    result.bxdfSampleType = BXDFType(BXDF_GLOSSY | BXDF_REFLECTION);
    result.directionIn = in;
    return result;
}
