#include "PlasticBxDF.h"
#include "MicrofacetDistribution.h"
#include "Fresnel.h"
#include "CoreLayer/Geometry/Frame.h"
#include "CoreLayer/Math/Warp.h"

double Plastic::pdf(const Vec3d &out, const Vec3d &in) const {
    //Todo: check this with sample type.
    bool sampleDiffuseR = true, sampleSpecularR = true;
    if (CosTheta(out) <= 0 || CosTheta(in) <= 0)
        return 0;
    double specProb, diffuseProb;
    if (sampleDiffuseR && sampleSpecularR) {
        specProb = Fresnel::dielectricReflectance(1 / ior, out.z);
        diffuseProb = _avgTransmittance * (1 - specProb);
        specProb /= (specProb + diffuseProb);
        if (isMirrorReflect(out, in))
            return specProb;
        else
            return (1 - specProb) * SquareToCosineHemispherePdf(in);
    } else if (sampleSpecularR) {
        return isMirrorReflect(out, in) ? 1 : 0;
    } else if (sampleDiffuseR)
        return SquareToCosineHemispherePdf(in);
    else
        return 0;
}

double Plastic::getRoughness() const {
    return _diffuseFresnel;
}

BxDFSampleResult Plastic::sample(const Vec3d &out, const Point2d &sample) const {
    //  bool sampleSpecularR = (event.requestType & (BSDF_SPECULAR | BSDF_REFLECTION)) == (BSDF_SPECULAR | BSDF_REFLECTION);
    //  bool sampleDiffuseR = (event.requestType & (BSDF_DIFFUSE | BSDF_REFLECTION)) == (BSDF_DIFFUSE | BSDF_REFLECTION);

    bool sampleSpecular = true, sampleDiffuseR = true;
    BxDFSampleResult result;
    if (CosTheta(out) <= 0) {
        result.pdf = 0;
        return {};
    }

    double specProb;
    auto F = Fresnel::dielectricReflectance(1 / ior, out.z);
    if (sampleDiffuseR && sampleSpecular) {
        specProb = F;
        double diffuseProb = _avgTransmittance * (1 - F);
        specProb /= (specProb + diffuseProb);
    } else if (sampleSpecular)
        specProb = 1;
    else
        specProb = 0;
    if (sample[0] < specProb) {
        result.directionIn = Frame::reflect(out);
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_SPECULAR);
        result.pdf = specProb;
        result.s = Spectrum(F)/result.directionIn.z;
        //        event.wi = Frame::Reflect(out);
        //        event.sampleType = BXDFType(BSDF_REFLECTION | BSDF_SPECULAR);
        //        event.pdf = specProb;
        //        return Spectrum(F);
    } else {

        double remapSample0 = (sample[0] - specProb) / (1 - specProb);
        Point2d remapSample(remapSample0, sample[1]);
        result.directionIn = SquareToCosineHemisphere(remapSample);
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_DIFFUSE);
        double eta = 1 / ior;
        double FOut = Fresnel::dielectricReflectance(eta, AbsCosTheta(out));
        double FIn = Fresnel::dielectricReflectance(eta, AbsCosTheta(result.directionIn));
        result.pdf = (1-specProb) * SquareToCosineHemispherePdf(result.directionIn);
        result.s = diffuseR * (1.f - FOut) * (1.f - FIn) * eta * eta *   INV_PI /
                   (Spectrum(1.f) - diffuseR * _diffuseFresnel);
        if (!_scaledSigmaA.isBlack())
            result.s *= exp(_scaledSigmaA * (-1.0f / out.z - 1.0f / result.directionIn.z));
    }
    return result;
}

Spectrum Plastic::f(const Vec3d &out, const Vec3d &in) const {
    bool sampleSpecular = true, sampleDiffuse = true;

    if (out.z <= 0 || in.z <= 0)
        return {0};
    double eta = 1 / ior;
    double FOut = Fresnel::dielectricReflectance(eta, out.z);

    if (isMirrorReflect(out, in) && sampleSpecular)
        return Spectrum(FOut)/in.z;
    else if (sampleDiffuse) {
        Spectrum specularContrib(0);
        double FIn = Fresnel::dielectricReflectance(eta, AbsCosTheta(in));
        Spectrum brdf = diffuseR * (1.f - FOut) * (1.f - FIn) * eta * eta  * INV_PI /
                        (Spectrum(1.f) - diffuseR * _diffuseFresnel);
        if (!_scaledSigmaA.isBlack())
            brdf *= exp(_scaledSigmaA * (-1.0f / out.z - 1.0f / in.z));
        return brdf;
    }

    //
    //    if ( out.z <= 0 || in.z <= 0 ) {
    //        return Spectrum(0);
    //    }
    //    double FOut = Fresnel::dielectricReflectance(1 / ior, out.z);
    //
    //    Spectrum specularContrib(0);
    //    if ( abs(out.z - in.z) < EPSILON ) {
    //        specularContrib = specularR / in.z;
    //    }
    //
    //    double FIn = Fresnel::dielectricReflectance(1 / ior, in.z);
    //
    //    Spectrum diffuseContrib = diffuseR * ( 1 - FOut ) * ( 1 - FIn ) * ( 1 / ( ior * ior ) ) / M_PI;
    //
    //    return specularContrib + diffuseContrib;
}


Plastic::Plastic(const Spectrum &diffuseR, double ior, double diffuseFresnel, double avgTransmittance, const Spectrum &scaledSigmaA)
    : diffuseR(diffuseR), ior(ior), _diffuseFresnel(diffuseFresnel), _avgTransmittance(avgTransmittance), _scaledSigmaA(scaledSigmaA)
{}

double RoughPlastic::pdf(const Vec3d &out, const Vec3d &in) const {
//    if (CosTheta(out) <= 0 || CosTheta(in) <= 0)
//        return 0;
//    vec3 wh = normalize(out + in);
//    auto glossyProb = F;
//    Float diffProb = (1 - glossyProb) * _avgTransmittance;
//    glossyProb  /=(glossyProb + diffProb);
//    diffProb = 1 - glossyProb;
//    vec2 alphaXY = getAlphaXY(event);
//    Float D = m_distrib->Pdf(event.wo, wh, alphaXY);
//    glossyProb *= D / (4 * absDot(out, wh));
//    diffProb *= in.z / Constant::PI;
//    return glossyProb + diffProb;

    auto glossyProb = Fresnel::dielectricReflectance(1/ior, out.z);
    double diffProb = (1 - glossyProb) * _avgTransmittance;
    glossyProb  /=(glossyProb + diffProb);
    diffProb = 1 - glossyProb;

    Vec3d wh = normalize(out + in);
    glossyProb *= distrib->Pdf(out, wh, alphaXY) / (4 * dot(wh, out));

    diffProb *= SquareToCosineHemispherePdf(in);

    return glossyProb + diffProb;
}

BxDFSampleResult RoughPlastic::sample(const Vec3d &out, const Point2d &sample) const {
    BxDFSampleResult result;
    auto glossyProb = Fresnel::dielectricReflectance(1/ior, out.z);
    double diffProb = (1 - glossyProb) * _avgTransmittance;
    glossyProb  /=(glossyProb + diffProb);
    if (sample[0] < glossyProb) {
        double remapSample0 = (glossyProb - sample[0]) / glossyProb;
        Point2d remapSample(remapSample0, sample[1]);
        Vec3d wh = distrib->Sample_wh(out, remapSample, alphaXY);
        result.directionIn = Frame::reflect(out, wh);
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    } else {
        double remapSample0 = (sample[0] - glossyProb) / (1 - glossyProb);
        Point2d remapSample(remapSample0, sample[1]);
        result.directionIn = SquareToCosineHemisphere(remapSample);
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_DIFFUSE);
    }
    result.pdf = pdf(out, result.directionIn);
    result.s = f(out, result.directionIn);
    return result;
}

Spectrum RoughPlastic::f(const Vec3d &out, const Vec3d &in) const {
    if (out.z <= 0 || in.z <= 0) {
        return Spectrum(0);
    }
    Vec3d wh = normalize(out + in);
    double  eta = 1/ior;
    double FOut = Fresnel::dielectricReflectance(eta, dot(wh, out));
    double D = distrib->D(wh, alphaXY);
    double G = distrib->G(out, in, alphaXY);

    Spectrum glossyContrib = FOut * D * G / (4 * out.z * in.z);

    double FIn = Fresnel::dielectricReflectance(eta, in.z);
    Spectrum    diffuseContrib =diffuseR * (1.f - FOut) * (1.f - FIn) * eta * eta  * INV_PI /
                              (Spectrum(1.f) - diffuseR * _diffuseFresnel);
    if (!_scaledSigmaA.isBlack())
        diffuseContrib *= exp(_scaledSigmaA * (-1.0f / out.z - 1.0f / in.z));;
    return glossyContrib + diffuseContrib;
}

double RoughPlastic::getRoughness() const {
    return (1-_diffuseFresnel) * (alphaXY.x+alphaXY.y)/2 + _diffuseFresnel;
}


RoughPlastic::RoughPlastic(const Spectrum &diffuseR, double ior, double diffuseFresnel,
                           double avgTransmittance, const Spectrum &scaledSigmaA,double _uRoughness,
                           double _vRoughness, const std::shared_ptr<MicrofacetDistribution> &_distrib):
                          distrib(_distrib),diffuseR(diffuseR), ior(ior), _diffuseFresnel(diffuseFresnel), _avgTransmittance(avgTransmittance), _scaledSigmaA(scaledSigmaA)
{
    alphaXY = Vec2d(distrib->roughnessToAlpha(_uRoughness), distrib->roughnessToAlpha(_vRoughness));

}
