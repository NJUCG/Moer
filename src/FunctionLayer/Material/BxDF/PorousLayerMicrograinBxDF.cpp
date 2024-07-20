#include "PorousLayerMicrograinBxDF.h"
#include "FastMath.h"
#include "Fresnel.h"
#include "CoreLayer/Geometry/Frame.h"

inline Vec3d R0ToEta(Spectrum R0) {
    Vec3d eta;
    for (int i = 0; i < 3; ++i) {
        eta[i] = (fm::sqrt(R0[i]) + 1) / (-fm::sqrt(R0[i]) + 1);
    }
    return eta;
}
ConductorMicrograinBxDF::ConductorMicrograinBxDF(Spectrum _R0, Vec3d _k, double _tau0, double _beta)
    : MicrograinBxDF(_tau0, _beta), R0(_R0), k(_k) {
}

Spectrum ConductorMicrograinBxDF::f(const Vec3d &out, const Vec3d &in) const {
    Vec3d eta = R0ToEta(R0);
    Vec3d half = normalize(in + out);
    auto Fr = Fresnel::conductorReflectance(eta, k, dot(half, in));
    double denominator = 4 * CosTheta(out);
    return (Fr * distrib->D(half, {tau0, beta}) * distrib->G(out, in, {tau0, beta})) / denominator;
}

double ConductorMicrograinBxDF::pdf(const Vec3d &out, const Vec3d &in) const {
    if (CosTheta(out) < 0 || CosTheta(in) < 0) return 0;
    Vec3d half = normalize(in + out);
    return distrib->Pdf(out, half, {tau0, beta}) / (4 * dot(out, half));
}

BxDFSampleResult ConductorMicrograinBxDF::sample(const Vec3d &out, const Point2d &sample) const {
    BxDFSampleResult result{};
    if (CosTheta(out) < 0) {
        return result;
    }
    Vec3d wh = distrib->Sample_wh(out, sample, {tau0, beta});
    Vec3d in = Frame::reflect(out, wh);
    if (dot(wh, out) < 0 || CosTheta(in) < 0) {
        result.s = Spectrum(0);
        return result;
    }
    result.pdf = pdf(out, in);
    result.s = f(out, in);
    result.directionIn = in;
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    return result;
}

PlasticMicrograinBxDF::PlasticMicrograinBxDF(Spectrum _R0, Spectrum _kd, double _tau0, double _beta)
    : MicrograinBxDF(_tau0, _beta), R0(_R0), kd(_kd) {
    distrib = std::make_shared<MicrograinDistribution>();
}

Spectrum PlasticMicrograinBxDF::f(const Vec3d &out, const Vec3d &in) const {
    Vec3d eta = R0ToEta(R0);
    Vec3d half = normalize(in + out);
    Spectrum Fr = Fresnel::dielectricReflectance(eta, dot(half, in));

    double denominator = 4 * CosTheta(out);
    auto specular = (Fr * distrib->D(half, {tau0, beta}) * distrib->G(out, in, {tau0, beta})) / denominator;

    auto Ti = -1. * Fresnel::dielectricReflectance(eta, CosTheta(in)) + 1.;
    auto To = -1. * Fresnel::dielectricReflectance(eta, CosTheta(out)) + 1.;
    auto diffuse = To * Ti * kd / fm::pi_d * CosTheta(in);

    return specular + diffuse;
}

double PlasticMicrograinBxDF::pdf(const Vec3d &out, const Vec3d &in) const {
    if (CosTheta(out) < 0 || CosTheta(in) < 0) return 0;
    Vec3d half = normalize(out + in);
    return distrib->Pdf(out, half, {tau0, beta}) / (4 * dot(out, half));
}

BxDFSampleResult PlasticMicrograinBxDF::sample(const Vec3d &out, const Point2d &sample) const {
    BxDFSampleResult result{};
    if (CosTheta(out) < 0) {
        return result;
    }
    Vec3d wh = distrib->Sample_wh(out, sample, {tau0, beta});
    Vec3d in = Frame::reflect(out, wh);
    if (dot(wh, out) < 0 || CosTheta(in) < 0) {
        result.s = Spectrum(0);
        return result;
    }
    result.pdf = pdf(out, in);
    result.s = f(out, in);
    result.directionIn = in;
    result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    return result;
}
