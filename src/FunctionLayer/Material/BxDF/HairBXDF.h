#include "BxDF.h"
#include "CoreLayer/Adapter/JsonUtil.h"


const int pMax = 3;

/// Hair Bsdf


struct HairAttribute {
    double scaleAngle;
    double melaninRatio;
    double melaninConcentration;
    bool overrideSigmaA;
    Vec3d sigmaA;
};

class Hair : public  BxDF{
public:
    double pdf(const Vec3d & out, const Vec3d & in) const override;
    Hair(const HairAttribute * attr,double roughness,double h);
    BxDFSampleResult sample(const Vec3d & out, const Point2d & sample) const override;
    Spectrum f(const Vec3d & out, const Vec3d & in) const override;

protected:
    double M(double v,double sinThetaO,double sinThetaI,double cosThetaO,double cosThetaI) const ;
    double sampleM(double v,double sinThetaO,double cosThetaO,double xi1,double xi2 ) const ;

    double NR(double beta,double cosO,double phi,double h) const;
    Vec3d  NP(double beta,double cosO,double phi,int p,double h) const;
    double D(double beta,double phi) const;

    std::array<double, pMax + 1> ComputeApPdf(double cosThetaO,double h) const;


private:
    double _scaleAngle;
    double _roughness;
    double _betaR,_betaTT,_betaTRT;
    double _vR,_vTT,_vTRT;
    Vec3d _sigmaA;
    const double _eta = 1.55;
    double h;
};

// https://fgiesen.wordpress.com/2009/12/13/decoding-morton-codes/
static uint32_t Compact1By1(uint32_t x) {
    // TODO: as of Haswell, the PEXT instruction could do all this in a
    // single instruction.
    // x = -f-e -d-c -b-a -9-8 -7-6 -5-4 -3-2 -1-0
    x &= 0x55555555;
    // x = --fe --dc --ba --98 --76 --54 --32 --10
    x = (x ^ (x >> 1)) & 0x33333333;
    // x = ---- fedc ---- ba98 ---- 7654 ---- 3210
    x = (x ^ (x >> 2)) & 0x0f0f0f0f;
    // x = ---- ---- fedc ba98 ---- ---- 7654 3210
    x = (x ^ (x >> 4)) & 0x00ff00ff;
    // x = ---- ---- ---- ---- fedc ba98 7654 3210
    x = (x ^ (x >> 8)) & 0x0000ffff;
    return x;
}

static Point2d DemuxDouble(double f) {
    uint64_t v = f * (1ull << 32);
    uint32_t bits[2] = {Compact1By1(v), Compact1By1(v >> 1)};
    return {bits[0] / double(1 << 16), bits[1] / double(1 << 16)};
}