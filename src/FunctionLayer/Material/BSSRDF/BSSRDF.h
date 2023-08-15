/**
* @file BSSRDF.h
* @author orbitchen
* @brief BSSRDF.
* @version 0.1
* @date 2022-04-30
*
* @copyright NJUMeta (c) 2022
* www.njumeta.com
*
*/
#pragma once

#include "FunctionLayer/Material/BxDF/BxDF.h"

class Scene;
class Intersection;

static void coordinateSystem(const Vec3d &a, Vec3d &b, Vec3d &c) {
    if (std::abs(a.x) > std::abs(a.y)) {
        float invLen = 1.0f / std::sqrt(a.x * a.x + a.z * a.z);
        c = Vec3d(a.z * invLen, 0.0f, -a.x * invLen);
    } else {
        float invLen = 1.0f / std::sqrt(a.y * a.y + a.z * a.z);
        c = Vec3d(0.0f, a.z * invLen, -a.y * invLen);
    }
    Vec3d _a(a.x, a.y, a.z);
    b = cross(_a, c);
}

double FresnelMoment1(double invEta);

double FresnelMoment2(double invEta);

class BSSRDFAdapterMaterial;

class BSSRDF {
public:
    virtual Spectrum
    sampleS(const Scene &scene, double u1, Point2d u2, Intersection *pi, Spectrum *pdf, Vec3d &out) const = 0;

    BSSRDF(double eta) : eta(eta) {}

    ///Used to determine whether the sampling point is on an object
    // void setBSDF(const BSDF * _bsdf) {bsdf = _bsdf;}
    //    const BSDF * bsdf;
    double eta;

protected:
    const Intersection *its;
};

///SeparableBSSRDF
class SeparableBSSRDF : public BSSRDF {
    friend class BSSRDFAdapater;

public:
    SeparableBSSRDF(double eta, const Intersection &po);

    Spectrum sampleS(const Scene &scene, double u1, Point2d u2, Intersection *pi, Spectrum  *pdf, Vec3d &out) const override;

    Spectrum sampleSp(const Scene &scene, double u1, const Point2d &u2, Intersection *pi,
                      Spectrum  *pdf, Vec3d &out) const;

    Spectrum  pdfSP(const Intersection &pi) const;

    Spectrum Sw(const Vec3d &w) const;

    // SeparableBSSRDF Interface
    virtual Spectrum Sr(double d) const = 0;

    virtual double sampleSr(int ch, double u) const = 0;

    virtual Spectrum  pdfSr(double r) const = 0;

    virtual double maxSr(int ch) const = 0;

protected:
    const Intersection &po;
    Vec3d ns, ss, ts;
    std::shared_ptr<BSSRDFAdapterMaterial> adapterMaterial;
};

struct BSSRDFTable {
    const int nRhoSamples, nRadiusSamples;
    std::unique_ptr<double[]> rhoSamples, radiusSamples;
    std::unique_ptr<double[]> profile;
    std::unique_ptr<double[]> rhoEff;
    std::unique_ptr<double[]> profileCDF;

    // BSSRDFTable Public Methods
    BSSRDFTable(int nRhoSamples, int nRadiusSamples);

    inline double EvalProfile(int rhoIndex, int radiusIndex) const {
        return profile[rhoIndex * nRadiusSamples + radiusIndex];
    }
};

class TabelBSSRDF : public SeparableBSSRDF {
public:
    Spectrum Sr(double d) const override;

    double sampleSr(int ch, double u) const override;

    Spectrum pdfSr(double r) const override;

    double maxSr(int ch) const override;

    TabelBSSRDF(double eta, Spectrum sigmaA, Spectrum sigmaS, double g, const BSSRDFTable &table, const Intersection &po);

private:
    Spectrum sigmaA, sigmaS;
    const BSSRDFTable &table;
    double g;
};

class BSSRDFAdapter : public BxDF {
public:
    BSSRDFAdapter(const SeparableBSSRDF *bssrdf) : bssrdf(bssrdf) {}

    double pdf(const Vec3d &out, const Vec3d &in) const override {
        if (out.z * in.z < 0)
            return 0.0f;
        return AbsCosTheta(in) * INV_PI;
    }

protected:
    double eta(const Vec3d &out, const Vec3d &in) const override {
        if(out.z * in.z >0) return  1;
        return out.z < 0 ? bssrdf->eta : 1 / bssrdf->eta;
    }
    BxDFSampleResult sample(const Vec3d &out, const Point2d &sample) const override {
        BxDFSampleResult result;
        auto in = SquareToCosineHemisphere(sample);
        if (out.z < 0) in.z *= -1;
        result.directionIn = in;
        result.bxdfSampleType = BXDFType(BXDF_DIFFUSE | BXDF_REFLECTION);
        result.pdf = pdf(out, in);
        result.s = f(out, in);
        return result;
    }

    Spectrum f(const Vec3d &out, const Vec3d &in) const override {
        if (out.z * in.z < 0)
            return {0};
        return bssrdf->Sw(in);
    }

    const SeparableBSSRDF *bssrdf;
};

static double BeamDiffusionMultScattering(double sigmaS, double sigmaA, double g, double eta, double r);

static double BeamDiffusionSingleScattering(double sigmaS, double sigmaA, double g, double eta, double r);

void computeBeamDiffusionBSSRDF(double g, double eta, BSSRDFTable *table);

void SubsurfaceFromDiffuse(const BSSRDFTable &t, const Spectrum &rhoEff,
                           const Spectrum &mfp, Spectrum *sigma_a,
                           Spectrum *sigma_s);