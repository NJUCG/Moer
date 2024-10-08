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
        double resample = (1.-sample[0]) / (1.-sampleWeight);
        result = bulkBxDF->sample(out, {resample, sample[1]});
        chooseBulkLobe = true;
    }
    double w = getMicrograinWeight(tau0, beta, CosTheta(result.directionIn), CosTheta(out));
     
    if (chooseBulkLobe) {
        Spectrum micrograin_f = micrograinBRDF->f(out, result.directionIn);
        result.s = (1 - w) * result.s + w*micrograin_f;
        double micrograin_pdf = micrograinBRDF->pdf(out, result.directionIn);
        if (micrograin_pdf > 0) {
            double misw = 1 - w; 
            result.s *= misw;
        }
        result.pdf *= 1.-sampleWeight;
    } else {
        Spectrum bulk_f = bulkBxDF->f(out, result.directionIn);
        result.s = w * result.s + (1 - w) * bulk_f;
        double bulk_pdf = bulkBxDF->pdf(out, result.directionIn); 
        if (bulk_pdf > 0) {
            double misw = w; 
            result.s *= misw;
        }
        result.pdf *= sampleWeight;   
    }
   
    return result;

}
