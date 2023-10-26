#include "HairBXDF.h"
#include "Fresnel.h"
#include "CoreLayer/Geometry/Geometry.h"
#include "BxDF.h"
#include <iostream>

static double trigInverse(double x) {
    return std::min(std::sqrt(std::max(1.0 - x*x, 0.0)), 1.0);

}

static double I0(double x) {
    double result = 1.0;
    double xSq = x * x;
    double xi = xSq;
    double denom = 4.0;
    for ( int i = 1 ; i <= 10 ; ++ i ) {
        result += xi / denom;
        xi *= xSq;
        denom *= 4.0 * double(( i + 1 ) * ( i + 1 ));
    }
    return result;
}

static double Phi(double gammaI, double gammaT, int p) {
    return 2.0 * p * gammaT - 2.0 * gammaI + p * M_PI;
}


static double Gussian(double beta, double theta) {
    return std::exp(- theta * theta / ( 2.0 * beta * beta )) / ( std::sqrt(2.0 * M_PI) * beta );
}


inline double Logistic(double x, double s) {
    x = std::abs(x);
    return std::exp(- x / s) / ( s * (1 + std::exp(- x / s)) * (1 + std::exp(- x / s)) );
}

inline double LogisticCDF(double x, double s) {
    return 1 / ( 1 + std::exp(- x / s) );
}


inline double TrimmedLogistic(double x, double s, double a, double b) {
    while ( x > M_PI ) x -= 2 * M_PI;
    while ( x < - M_PI ) x += 2 * M_PI;
    return Logistic(x, s) / ( LogisticCDF(b, s) - LogisticCDF(a, s) );
}

static double SampleTrimmedLogistic(double u, double s, double a, double b) {
    double k = LogisticCDF(b, s) - LogisticCDF(a, s);
    double x = - s * std::log(1 / ( u * k + LogisticCDF(a, s) ) - 1);
    return clamp(x, a, b);
}

static std::array <Spectrum , pMax + 1 > Ap(double cosThetaO, double eta, double h,
                                         const Spectrum & T) {
    std::array < Spectrum, pMax + 1 > ap;
    double cosGammaO = trigInverse(1 - h * h);
    double cosTheta = cosThetaO * cosGammaO;
    double f = Fresnel::dielectricReflectance(1 / eta, cosTheta);
    ap[0] = Spectrum(f);
    ap[1] = (1-f) * (1-f) * T;
    for ( int p = 2 ; p < pMax ; ++ p ) ap[p] = ap[p - 1] * T * f;
    return ap;
}

static double logI0(double x)
{
    if (x > 12.0f)
        // More stable evaluation of log(I0(x))
        // See also https://publons.com/discussion/12/
        return x + 0.5f*(std::log(1.0f/(M_PI * 2 *x)) + 1.0f/(8.0f*x));
    else
        return std::log(I0(x));
}

double Hair::M(double v, double sinThetaO, double sinThetaI, double cosThetaO, double cosThetaI) const {

    double b = sinThetaO * sinThetaI / v;
    double a = cosThetaI * cosThetaO / v;
    if (v < 0.1)
        return std::exp(-b + logI0(a) - 1.0f / v + 0.6931f + std::log(1.0f / (2.0f * v)));
    double csch = 2 / (exp(1 / v) - exp(-1 / v));
    return csch * exp(-b) * I0(a);
}


double Hair::NR(double beta, double cosO, double phi, double h) const {
    double gammaI = std::asin(clamp(h, - 1.0, 1.0));
    double deltaPhi = phi + 2.0 * gammaI;
    deltaPhi = std::fmod(deltaPhi, 2 * M_PI);
    if ( deltaPhi < 0.0 )
        deltaPhi += 2 * M_PI;
    return TrimmedLogistic(deltaPhi, beta, - M_PI, M_PI) *
           Fresnel::dielectricReflectance(1.0 / _eta, cosO * cos(gammaI));
}

Vec3d Hair::NP(double beta, double cosThetaD, double phi, int p, double h) const {
    double iorPrime = std::sqrt(_eta * _eta - ( 1.0 - cosThetaD * cosThetaD )) / cosThetaD;
    double cosThetaT = std::sqrt(1.0 - ( 1.0 - cosThetaD * cosThetaD ) * (1.0 / _eta / _eta));
    Vec3d sigmaAPrime = _sigmaA / cosThetaT;
    double gammaI = std::asin(clamp(h, - 1.0, 1.0));
    double gammaT = std::asin(clamp(h / iorPrime, - 1.0, 1.0));
    double l = 2.0 * std::cos(gammaT);

    double f = Fresnel::dielectricReflectance(1.0 / _eta, cosThetaD * trigInverse(h));
    Vec3d TDistance = -sigmaAPrime * l;

    Vec3d T = Vec3d(exp(TDistance.x),exp(TDistance.y),exp(TDistance.z));
    Vec3d Aph = ( 1.0 - f ) * ( 1.0 - f ) * T;
    for ( int i = 1 ; i < p ; ++ i )
        Aph = Aph * f * T;

    double deltaPhi = phi - Phi(gammaI, gammaT, p);
    deltaPhi = std::fmod(deltaPhi, 2 * M_PI);
    if ( deltaPhi < 0.0 )
        deltaPhi += 2 * M_PI;
    return Aph * TrimmedLogistic(deltaPhi, beta, - M_PI, M_PI);
}

double Hair::D(double beta, double phi) const {
    double result = 0.0;
    double delta;
    double shift = 0.0;
    do {
        delta = Gussian(beta, phi + shift) + Gussian(beta, phi - shift - 2 * M_PI);
        result += delta;
        shift += 2 * M_PI;
    } while ( delta > 1e-4f );
    return result;
}


//return sinTheta
double Hair::sampleM(double v, double sinThetaO, double cosThetaO, double xi1, double xi2) const {
    double cosTheta = 1.0 + v * std::log(xi1 + ( 1.0 - xi1 ) * std::exp(- 2.0 / v));
    double sinTheta = trigInverse(cosTheta);
    double cosPhi = std::cos(2 * M_PI * xi2);
    return clamp(-cosTheta * sinThetaO + sinTheta * cosPhi * cosThetaO,-1.0,1.0);
}

std::array < double, pMax + 1 > Hair::ComputeApPdf(double cosThetaO, double h) const {
    double sinThetaO = trigInverse(cosThetaO);

    double sinThetaT = sinThetaO / _eta;
    double cosThetaT = trigInverse(sinThetaT);

    double etap = std::sqrt(_eta * _eta -  sinThetaO * sinThetaO) / cosThetaO;
    double sinGammaT = h / etap;
    double cosGammaT = trigInverse(sinGammaT);

    RGB3 expSigmaA = RGB3(exp(_sigmaA.x), exp(_sigmaA.y),exp(_sigmaA.z));
    RGB3 T = expSigmaA * ( 2 * cosGammaT / cosThetaT );
    std::array < Spectrum , pMax + 1 > ap = Ap(cosThetaO, _eta, h, Spectrum(T));

    // Compute $A_p$ PDF from individual $A_p$ terms
    std::array < double, pMax + 1 > apPdf;
    double sumY =
        std::accumulate(ap.begin(), ap.end()-1, 0.0,
                        [](double s, const Spectrum & ap) { return s + ap.luminance(); });
    for ( int i = 0 ; i < pMax ; ++ i ) apPdf[i] = ap[i].luminance() / sumY;
    return apPdf;
}


double Hair::pdf(const Vec3d & out, const Vec3d & in) const {
    double sinThetaO = out.y;
    double costhetaO = trigInverse(sinThetaO);
    double thetaO = std::asin(clamp(sinThetaO, - 1.0, 1.0));

    double sinThetaI = in.y;
    double cosThetaI = sqrt(1 - sinThetaI * sinThetaI);
    double thetaI = std::asin(clamp(sinThetaI, - 1.0, 1.0));
    //First choose a lobe to sample
    std::array < double, pMax + 1 > apPdf = ComputeApPdf(costhetaO, h);

    double thetaOR = thetaO - 2.0 * _scaleAngle;
    double thetaOTT = thetaO + _scaleAngle;
    double thetaOTRT = thetaO + 4 * _scaleAngle;

    double phiI = std::atan2(in.x, in.z);
    double phiO = std::atan2(out.x, out.z);
    double phi = phiI - phiO;

    double cosThetaD = cos(0.5 * ( std::asin(sinThetaI) - thetaO ));
    double iorPrime = std::sqrt(_eta * _eta - ( 1.0 - cosThetaD * cosThetaD )) / cosThetaD;
    double gammaI = std::asin(clamp(h, - 1.0, 1.0));
    double gammaT = std::asin(clamp(h / iorPrime, - 1.0, 1.0));

    double pdf = 0;
    pdf += M(_vR, sin(thetaOR), sinThetaI, cos(thetaOR), cosThetaI) * apPdf[0] * TrimmedLogistic(phi- Phi(gammaI,gammaT,0),_betaR,-M_PI,M_PI);
    pdf += M(_vTT, sin(thetaOTT), sinThetaI, cos(thetaOTT), cosThetaI) * apPdf[1] * TrimmedLogistic(phi- Phi(gammaI,gammaT,1),_betaR,-M_PI,M_PI);
    pdf += M(_vTRT, sin(thetaOTRT), sinThetaI, cos(thetaOTRT), cosThetaI) * apPdf[2] *TrimmedLogistic(phi- Phi(gammaI,gammaT,2),_betaR,-M_PI,M_PI);
    return pdf;
}

BxDFSampleResult Hair::sample(const Vec3d & out, const Point2d & sample) const {
    BxDFSampleResult result;
    //Hair-samplineg requires 4 randoms.
    Point2d u0 = DemuxDouble(sample[0]), u1 = DemuxDouble(sample[1]);
    double sinThetaO = out.y;
    double costhetaO = trigInverse(sinThetaO);
    double thetaO = std::asin(clamp(sinThetaO, - 1.0, 1.0));
    //First choose a lobe to sample
    std::array < double, pMax + 1 > apPdf = ComputeApPdf(costhetaO, h);
    int p;
    for ( p = 0 ; p < pMax ; ++ p ) {
        if ( u0[0] <= apPdf[p] )
            break;
        u0[0] -= apPdf[p];
    }
    double theta, v;
    if ( p == 0 ) {
        theta = thetaO - 2 * _scaleAngle;
        v = _vR;
    } else if ( p == 1 ) {
        theta = thetaO + _scaleAngle;
        v = _vTT;
    } else if ( p == 2 ) {
        theta = thetaO + 4 * _scaleAngle;
        v = _vTRT;
    } else { throw ( "Invalid P" ); }

    double sinThetaI = sampleM(v, sin(theta), cos(theta), u1[0], u1[1]);
    double cosThetaI = trigInverse(sinThetaI);

    double phiO = std::atan2(out.x, out.z);
    double deltaphi;
    double cosThetaD = cos(0.5 * ( std::asin(clamp(sinThetaI,-1.0,1.0)) - thetaO ));
    double iorPrime = std::sqrt(_eta * _eta - ( 1.0 - cosThetaD * cosThetaD )) / cosThetaD;
    double gammaI = std::asin(clamp(h, - 1.0, 1.0));
    double gammaT = std::asin(clamp(h / iorPrime, - 1.0, 1.0));

    deltaphi = Phi(gammaI, gammaT, p) + SampleTrimmedLogistic(u0[1], v, - M_PI, M_PI);
    double phi = phiO + deltaphi;
    double sinPhi = sin(phi), cosPhi = cos(phi);

    result.directionIn = Vec3d(sinPhi * cosThetaI, sinThetaI, cosPhi * cosThetaI);
    result.pdf = pdf(out,result.directionIn);
    result.s = f(out,result.directionIn);
    result.bxdfSampleType = type;

    return result;
}

Spectrum Hair::f(const Vec3d & out, const Vec3d & in) const {

    double sinThetaO = out.y;
    double thetaO = std::asin(clamp(sinThetaO, - 1.0, 1.0));

    double sinThetaI = in.y;
    double cosThetaI = sqrt(1 - sinThetaI * sinThetaI);
    double thetaI = std::asin(clamp(sinThetaI, - 1.0, 1.0));

    double thetaD = ( thetaI - thetaO ) * 0.5;
    double cosThetaD = cos(thetaD);

    double thetaOR = thetaO - 2.0 * _scaleAngle;
    double thetaOTT = thetaO + _scaleAngle;
    double thetaOTRT = thetaO + 4 * _scaleAngle;

    double MR = M(_vR, sin(thetaOR), sinThetaI, cos(thetaOR), cosThetaI);
    double MTT = M(_vTT, sin(thetaOTT), sinThetaI, cos(thetaOTT), cosThetaI);
    double MTRT = M(_vTRT, sin(thetaOTRT), sinThetaI, cos(thetaOTRT), cosThetaI);

    double phi = std::atan2(in.x, in.z);
    if ( phi < 0.0 )
        phi += 2 * M_PI;

    Vec3d Nr = Vec3d(NR(_betaR, trigInverse(out.y), phi, h));
    Vec3d Ntt = NP(_betaR, cosThetaD, phi, 1, h);
    Vec3d Ntrt = NP(_betaR, cosThetaD, phi, 2, h);
    Vec3d fsum = MR * Nr + MTT * Ntt + MTRT * Ntrt;
    if (AbsCosTheta(in) > 0) fsum /= AbsCosTheta(in);
    // return Spectrum(RGB3(Nr.x,Nr.y,Nr.z))/AbsCosTheta(in);
    return Spectrum(RGB3(fsum.x,fsum.y,fsum.z));
}

const Vec3d eumelaninSigmaA = Vec3d(0.419, 0.697, 1.37);
const Vec3d pheomelaninSigmaA = Vec3d (0.187, 0.4, 1.05);

Hair::Hair(const HairAttribute * attr, double roughness,double  h) : _roughness(roughness),h(h){
    type= BXDFType(BXDF_GLOSSY | BXDF_TRANSMISSION | BXDF_REFLECTION);

    _scaleAngle =   attr->scaleAngle;
    _sigmaA = attr->sigmaA;
    if(!attr->overrideSigmaA){
        _sigmaA = attr->melaninConcentration * lerp(eumelaninSigmaA, pheomelaninSigmaA, attr->melaninRatio);
    }
    _betaR = std::max(M_PI * 0.5 * _roughness, 0.04);
    _betaTT = _betaR * 0.5f;
    _betaTRT = _betaR * 2.0;

    _vR = _betaR * _betaR;
    _vTT = _betaTT * _betaTT;
    _vTRT = _betaTRT * _betaTRT;
}
