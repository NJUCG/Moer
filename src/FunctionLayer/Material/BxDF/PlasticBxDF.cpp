#include "PlasticBxDF.h"
#include "MicrofacetDistribution.h"
#include "Fresnel.h"
#include "CoreLayer/Geometry/Frame.h"
#include "CoreLayer/Math/Warp.h"

double Plastic::pdf(const Vec3d & out, const Vec3d & in) const {
    if ( CosTheta(out) <= 0 || CosTheta(in) <= 0 )
        return 0;
    double lS = specularR.luminance(), lD = diffuseR.luminance();
    if ( lS + lD <= 0 ) {
        return 0;
    }
    double specProb = lS / ( lS + lD );
    double diffProb = 1 - specProb;

    if ( abs(out.z - in.z) > EPSILON ) {
        specProb = 0;
    }

    diffProb *= SquareToCosineHemispherePdf(in);

    return specProb + diffProb;
}

double Plastic::getRoughness() const {
    double lS = specularR.luminance(), lD = diffuseR.luminance();
    double specProb = lS / ( lS + lD );
    return 1 - specProb;
}

BxDFSampleResult Plastic::sample(const Vec3d & out, const Point2d & sample) const {
    BxDFSampleResult result;
    if ( CosTheta(out) <= 0 ) {
        result.pdf = 0;
        return {};
    }
    double lS = specularR.luminance(), lD = diffuseR.luminance();
    if ( lS + lD <= 0 ) {
        result.pdf = 0;
        return result;
    }
    double specProb = lS / ( lS + lD );
    if ( sample[0] < specProb ) {
        result.directionIn = Frame::reflect(out);
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_SPECULAR);
    } else {
        double remapSample0 = ( sample[0] - specProb ) / ( 1 - specProb );
        Point2d remapSample(remapSample0, sample[1]);
        result.directionIn = SquareToCosineHemisphere(remapSample);
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_DIFFUSE);
    }
    result.pdf = pdf(out, result.directionIn);
    result.s = f(out, result.directionIn);
    return result;
}

Spectrum Plastic::f(const Vec3d & out, const Vec3d & in) const {
    if ( out.z <= 0 || in.z <= 0 ) {
        return Spectrum(0);
    }
    double FOut = Fresnel::dielectricReflectance(1 / ior, out.z);

    Spectrum specularContrib(0);
    if ( abs(out.z - in.z) < EPSILON ) {
        specularContrib = specularR / in.z;
    }

    double FIn = Fresnel::dielectricReflectance(1 / ior, in.z);

    Spectrum diffuseContrib = diffuseR * ( 1 - FOut ) * ( 1 - FIn ) * ( 1 / ( ior * ior ) ) / M_PI;

    return specularContrib + diffuseContrib;
}

Plastic::Plastic(const Spectrum & _specularR, const Spectrum & _diffuseR, double _ior) : specularR(_specularR),
                                                                                       diffuseR(_diffuseR),
                                                                                       ior(_ior) {}


double RoughPlastic::pdf(const Vec3d & out, const Vec3d & in) const {
    if ( CosTheta(out) <= 0 || CosTheta(in) <= 0 )
        return 0;
    double lS = glossyR.luminance(), lD = diffuseR.luminance();
    if ( lS + lD <= 0 ) {
        return 0;
    }
    double glossyProb = lS / ( lS + lD );
    double diffProb = 1 - glossyProb;

    Vec3d wh = normalize(out + in);
    glossyProb *= distrib->Pdf(out,wh, alphaXY) / ( 4 * dot(wh, out) );

    diffProb *= SquareToCosineHemispherePdf(in);

    return glossyProb + diffProb;
}

BxDFSampleResult RoughPlastic::sample(const Vec3d & out, const Point2d & sample) const {
    BxDFSampleResult result;
    if ( CosTheta(out) <= 0 ) {
        result.pdf = 0;
        return {};
    }
    double lS = glossyR.luminance(), lD = diffuseR.luminance();
    if ( lS + lD <= 0 ) {
        result.pdf = 0;
        return result;
    }
    double specProb = lS / ( lS + lD );
    if ( sample[0] < specProb ) {
        double remapSample0 = ( specProb - sample[0] ) / specProb;
        Point2d remapSample(remapSample0, sample[1]);
        Vec3d wh = distrib->Sample_wh(out, remapSample, alphaXY);
        result.directionIn = Frame::reflect(out, wh);
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_GLOSSY);
    } else {
        double remapSample0 = ( sample[0] - specProb ) / ( 1 - specProb );
        Point2d remapSample(remapSample0, sample[1]);
        result.directionIn = SquareToCosineHemisphere(remapSample);
        result.bxdfSampleType = BXDFType(BXDF_REFLECTION | BXDF_DIFFUSE);
    }
    result.pdf = pdf(out, result.directionIn);
    result.s = f(out, result.directionIn);
    return result;
}

Spectrum RoughPlastic::f(const Vec3d & out, const Vec3d & in) const {
    if ( out.z <= 0 || in.z <= 0 ) {
        return Spectrum(0);
    }
    Vec3d wh = normalize(out + in);
    double FOut = Fresnel::dielectricReflectance(1 / ior, dot(wh,out));
    double D = distrib->D(wh, alphaXY);
    double G = distrib->G(out, in, alphaXY);

    Spectrum glossyContrib = FOut * D * G / ( 4 * out.z * in.z );

    double FIn = Fresnel::dielectricReflectance(1 / ior, in.z);
    Spectrum diffuseContrib = diffuseR * ( 1 - FOut ) * ( 1 - FIn ) * ( 1 / ( ior * ior ) ) / M_PI;
    return glossyContrib + diffuseContrib;
}

double RoughPlastic::getRoughness() const {
    double lS = glossyR.luminance(), lD = diffuseR.luminance();
    double glossyProb = lS / ( lS + lD );
    double glossyRoughness = (alphaXY[0] + alphaXY[1]) / 2.;
    return lerp(1., glossyRoughness, glossyProb);
}

RoughPlastic::RoughPlastic(const Spectrum & _glossyR, const Spectrum & _diffuseR, double _ior,
                           double _uRoughness, double _vRoughness,
                           const std::shared_ptr < MicrofacetDistribution > & _distrib) :
                                                                                      glossyR(_glossyR), diffuseR(_diffuseR), ior(_ior), distrib(_distrib) {
    alphaXY = Vec2d(distrib->roughnessToAlpha(_uRoughness), distrib->roughnessToAlpha(_vRoughness));
}


