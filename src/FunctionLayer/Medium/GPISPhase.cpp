#include "GPISPhase.h"
#include "FunctionLayer/Material/MaterialFactory.h"
#include "FunctionLayer/Intersection.h"

GPISPhase::GPISPhase(const Json &json) {
    auto transientMaterial = MaterialFactory::LoadMaterialFromJson(json);
    Intersection pseudoIts{};
    innerBxDF = transientMaterial->getBxDF(pseudoIts);
}

std::tuple<Spectrum, double, bool> GPISPhase::evalPhase(Vec3d wo, Vec3d wi, Point3d scatterPoint) const {

    auto phaseValue = innerBxDF->f(wo, wi) * CosTheta(wi);
    double phasePdf = innerBxDF->pdf(wo, wi);
    return {phaseValue, phasePdf, false};
}

std::tuple<Vec3d, Spectrum, double, bool> GPISPhase::samplePhase(Vec3d wo, Point3d scatterPoint, Point2d sample) const {
    auto bxdfSampleResult = innerBxDF->sample(wo, sample);
    return {bxdfSampleResult.directionIn, bxdfSampleResult.s * CosTheta(bxdfSampleResult.directionIn), bxdfSampleResult.pdf, BxDF::MatchFlags(bxdfSampleResult.bxdfSampleType, BXDF_SPECULAR)};
}
