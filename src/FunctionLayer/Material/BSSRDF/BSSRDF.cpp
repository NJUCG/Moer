/**
 * @file 
 * @author JunPing Yuan
 * @brief 
 * @version 0.1
 * @date 2023/1/18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "BSSRDF.h"
#include "FunctionLayer/Material/BSSRDFMaterial.h"
#include "FunctionLayer/Material/BxDF/Fresnel.h"
#include "FunctionLayer/Scene/Scene.h"
#include "FunctionLayer/Intersection.h"

template<typename Predicate>
int FindInterval(int size, const Predicate &pred) {
    int first = 0, len = size;
    while (len > 0) {
        int half = len >> 1, middle = first + half;
        // Bisect range based on value of _pred_ at _middle_
        if (pred(middle)) {
            first = middle + 1;
            len -= half + 1;
        } else
            len = half;
    }
    return clamp(first - 1, 0, size - 2);
}

bool CatmullRomWeights(int size, const double *nodes, double x, int *offset,
                       double *weights) {
    // Return _false_ if _x_ is out of bounds
    if (!(x >= nodes[0] && x <= nodes[size - 1])) return false;

    // Search for the interval _idx_ containing _x_
    int idx = FindInterval(size, [&](int i) { return nodes[i] <= x; });
    *offset = idx - 1;
    double x0 = nodes[idx], x1 = nodes[idx + 1];

    // Compute the $t$ parameter and powers
    double t = (x - x0) / (x1 - x0), t2 = t * t, t3 = t2 * t;

    // Compute initial node weights $w_1$ and $w_2$
    weights[1] = 2 * t3 - 3 * t2 + 1;
    weights[2] = -2 * t3 + 3 * t2;

    // Compute first node weight $w_0$
    if (idx > 0) {
        double w0 = (t3 - 2 * t2 + t) * (x1 - x0) / (x1 - nodes[idx - 1]);
        weights[0] = -w0;
        weights[2] += w0;
    } else {
        double w0 = t3 - 2 * t2 + t;
        weights[0] = 0;
        weights[1] -= w0;
        weights[2] += w0;
    }

    // Compute last node weight $w_3$
    if (idx + 2 < size) {
        double w3 = (t3 - t2) * (x1 - x0) / (nodes[idx + 2] - x0);
        weights[1] -= w3;
        weights[3] = w3;
    } else {
        double w3 = t3 - t2;
        weights[1] -= w3;
        weights[2] += w3;
        weights[3] = 0;
    }
    return true;
}

double SampleCatmullRom2D(int size1, int size2, const double *nodes1,
                          const double *nodes2, const double *values,
                          const double *cdf, double alpha, double u, double *fval = nullptr,
                          double *pdf = nullptr) {
    // Determine offset and coefficients for the _alpha_ parameter
    int offset;
    double weights[4];
    if (!CatmullRomWeights(size1, nodes1, alpha, &offset, weights)) return 0;

    // Define a lambda function to interpolate table entries
    auto interpolate = [&](const double *array, int idx) {
        double value = 0;
        for (int i = 0; i < 4; ++i)
            if (weights[i] != 0)
                value += array[(offset + i) * size2 + idx] * weights[i];
        return value;
    };

    // Map _u_ to a spline interval by inverting the interpolated _cdf_
    double maximum = interpolate(cdf, size2 - 1);
    u *= maximum;
    int idx =
        FindInterval(size2, [&](int i) { return interpolate(cdf, i) <= u; });

    // Look up node positions and interpolated function values
    double f0 = interpolate(values, idx), f1 = interpolate(values, idx + 1);
    double x0 = nodes2[idx], x1 = nodes2[idx + 1];
    double width = x1 - x0;
    double d0, d1;

    // Re-scale _u_ using the interpolated _cdf_
    u = (u - interpolate(cdf, idx)) / width;

    // Approximate derivatives using finite differences of the interpolant
    if (idx > 0)
        d0 = width * (f1 - interpolate(values, idx - 1)) /
             (x1 - nodes2[idx - 1]);
    else
        d0 = f1 - f0;
    if (idx + 2 < size2)
        d1 = width * (interpolate(values, idx + 2) - f0) /
             (nodes2[idx + 2] - x0);
    else
        d1 = f1 - f0;

    // Invert definite integral over spline segment and return solution

    // Set initial guess for $t$ by importance sampling a linear interpolant
    double t;
    if (f0 != f1)
        t = (f0 - std::sqrt(std::max((double)0, f0 * f0 + 2 * u * (f1 - f0)))) /
            (f0 - f1);
    else
        t = u / f0;
    double a = 0, b = 1, Fhat, fhat;
    while (true) {
        // Fall back to a bisection step when _t_ is out of bounds
        if (!(t >= a && t <= b)) t = 0.5f * (a + b);

        // Evaluate target function and its derivative in Horner form
        Fhat = t * (f0 +
                    t * (.5f * d0 +
                         t * ((1.f / 3.f) * (-2 * d0 - d1) + f1 - f0 +
                              t * (.25f * (d0 + d1) + .5f * (f0 - f1)))));
        fhat = f0 +
               t * (d0 +
                    t * (-2 * d0 - d1 + 3 * (f1 - f0) +
                         t * (d0 + d1 + 2 * (f0 - f1))));

        // Stop the iteration if converged
        if (std::abs(Fhat - u) < 1e-6f || b - a < 1e-6f) break;

        // Update bisection bounds using updated _t_
        if (Fhat - u < 0)
            a = t;
        else
            b = t;

        // Perform a Newton step
        t -= (Fhat - u) / fhat;
    }

    // Return the sample position and function value
    if (fval) *fval = fhat;
    if (pdf) *pdf = fhat / maximum;
    return x0 + width * t;
}

double IntegrateCatmullRom(int n, const double *x, const double *values,
                           double *cdf) {
    double sum = 0;
    cdf[0] = 0;
    for (int i = 0; i < n - 1; ++i) {
        // Look up $x_i$ and function values of spline segment _i_
        double x0 = x[i], x1 = x[i + 1];
        double f0 = values[i], f1 = values[i + 1];
        double width = x1 - x0;

        // Approximate derivatives using finite differences
        double d0, d1;
        if (i > 0)
            d0 = width * (f1 - values[i - 1]) / (x1 - x[i - 1]);
        else
            d0 = f1 - f0;
        if (i + 2 < n)
            d1 = width * (values[i + 2] - f0) / (x[i + 2] - x0);
        else
            d1 = f1 - f0;

        // Keep a running sum and build a cumulative distribution function
        sum += ((d0 - d1) * (1.f / 12.f) + (f0 + f1) * .5f) * width;
        cdf[i + 1] = sum;
    }
    return sum;
}

double InvertCatmullRom(int n, const double *x, const double *values, double u) {
    // Stop when _u_ is out of bounds
    if (!(u > values[0]))
        return x[0];
    else if (!(u < values[n - 1]))
        return x[n - 1];

    // Map _u_ to a spline interval by inverting _values_
    int i = FindInterval(n, [&](int i) { return values[i] <= u; });

    // Look up $x_i$ and function values of spline segment _i_
    double x0 = x[i], x1 = x[i + 1];
    double f0 = values[i], f1 = values[i + 1];
    double width = x1 - x0;

    // Approximate derivatives using finite differences
    double d0, d1;
    if (i > 0)
        d0 = width * (f1 - values[i - 1]) / (x1 - x[i - 1]);
    else
        d0 = f1 - f0;
    if (i + 2 < n)
        d1 = width * (values[i + 2] - f0) / (x[i + 2] - x0);
    else
        d1 = f1 - f0;

    // Invert the spline interpolant using Newton-Bisection
    double a = 0, b = 1, t = .5f;
    double Fhat, fhat;
    while (true) {
        // Fall back to a bisection step when _t_ is out of bounds
        if (!(t > a && t < b)) t = 0.5f * (a + b);

        // Compute powers of _t_
        double t2 = t * t, t3 = t2 * t;

        // Set _Fhat_ using Equation (8.27)
        Fhat = (2 * t3 - 3 * t2 + 1) * f0 + (-2 * t3 + 3 * t2) * f1 +
               (t3 - 2 * t2 + t) * d0 + (t3 - t2) * d1;

        // Set _fhat_ using Equation (not present)
        fhat = (6 * t2 - 6 * t) * f0 + (-6 * t2 + 6 * t) * f1 +
               (3 * t2 - 4 * t + 1) * d0 + (3 * t2 - 2 * t) * d1;

        // Stop the iteration if converged
        if (std::abs(Fhat - u) < 1e-6f || b - a < 1e-6f) break;

        // Update bisection bounds using updated _t_
        if (Fhat - u < 0)
            a = t;
        else
            b = t;

        // Perform a Newton step
        t -= (Fhat - u) / fhat;
    }
    return x0 + t * width;
}

// BSSRDF Utility Functions
double FresnelMoment1(double eta) {
    double eta2 = eta * eta, eta3 = eta2 * eta, eta4 = eta3 * eta,
           eta5 = eta4 * eta;
    if (eta < 1)
        return 0.45966f - 1.73965f * eta + 3.37668f * eta2 - 3.904945 * eta3 +
               2.49277f * eta4 - 0.68441f * eta5;
    else
        return -4.61686f + 11.1136f * eta - 10.4646f * eta2 + 5.11455f * eta3 -
               1.27198f * eta4 + 0.12746f * eta5;
}

double FresnelMoment2(double eta) {
    double eta2 = eta * eta, eta3 = eta2 * eta, eta4 = eta3 * eta,
           eta5 = eta4 * eta;
    if (eta < 1) {
        return 0.27614f - 0.87350f * eta + 1.12077f * eta2 - 0.65095f * eta3 +
               0.07883f * eta4 + 0.04860f * eta5;
    } else {
        double r_eta = 1 / eta, r_eta2 = r_eta * r_eta, r_eta3 = r_eta2 * r_eta;
        return -547.033f + 45.3087f * r_eta3 - 218.725f * r_eta2 +
               458.843f * r_eta + 404.557f * eta - 189.519f * eta2 +
               54.9327f * eta3 - 9.00603f * eta4 + 0.63942f * eta5;
    }
}

Spectrum  SeparableBSSRDF::pdfSP(const Intersection &pi) const {
    Vec3d d = po.position - pi.position;
    Frame f = Frame::FromZ(ns);
    auto dLocal = f.toLocal(d);
    auto nLocal = f.toLocal(pi.geometryNormal);

    // Compute BSSRDF profile radius under projection along each axis
    double rProj[3] = {std::sqrt(dLocal.y * dLocal.y + dLocal.z * dLocal.z),
                       std::sqrt(dLocal.z * dLocal.z + dLocal.x * dLocal.x),
                       std::sqrt(dLocal.x * dLocal.x + dLocal.y * dLocal.y)};

    // Return combined probability from all BSSRDF sampling strategies
    Spectrum pdf = 0; double axisProb[3] = {.25f, .25f, .5f};
    double chProb = 1 / double(3);
    for (int axis = 0; axis < 3; ++axis)
        for (int ch = 0; ch < 3; ++ch)
            pdf += pdfSr(rProj[axis]) * std::abs(nLocal[axis]) * chProb *
                   axisProb[axis];
    return pdf;
}

Spectrum
SeparableBSSRDF::sampleS(const Scene &scene, double u1, Point2d u2, Intersection *pi, Spectrum * pdf,
                         Vec3d &out) const {
    Spectrum s = sampleSp(scene, u1, u2, pi, pdf, out);
    pi->material = adapterMaterial;
    return s;
}

Spectrum
SeparableBSSRDF::sampleSp(const Scene &scene, double u1, const Point2d &u2, Intersection *pi,
                          Spectrum  *pdf, Vec3d &out) const {
    Frame f;
    if (u1 < 0.25f)
        f = Frame::FromX(ns);
    else if (u1 < 0.5f)
        f = Frame::FromY(ns);
    else
        f = Frame::FromZ(ns);

    // Choose spectral channel for BSSRDF sampling
    int ch = clamp((int)(u1 * 3), 0, 2);
    u1 = u1 * 3 - ch;

    double r = sampleSr(ch, u2[0]);
    //  *pdf = 1.0; return Spectrum(r);
    if (r < 0)
        return Spectrum(0);
    double phi = 2 * M_PI * u2[1];
    double rMax = maxSr(ch);
    if (r > rMax)
        return Spectrum(0);
    //    return Spectrum(rMax);
    double l = 2 * std::sqrt(rMax * rMax - r * r);
    Intersection base;

    Vec3d poDirBase = normalize(r * (f.s * std::cos(phi) + f.t * std::sin(phi)) -
                                l * f.n * 0.5f);
    Vec3d baseDirTarget = f.n;

    base.position = po.position + r * (f.s * std::cos(phi) + f.t * std::sin(phi)) -
                    l * f.n * 0.5f;
    Point3d pTarget = base.position + l * f.n;

    /// may intersect more than one point
    struct IntersectionChain {
        std::optional<Intersection> si;
        IntersectionChain *next;
        Vec3d rayDir;
    };
    IntersectionChain *chain = new IntersectionChain();
    IntersectionChain *ptr = chain;
    std::optional<Intersection> its;
    Ray ray = base.spawnRay(pTarget);
    double initialL = (pTarget - base.position).length();
    // double tHit = ray.timeMax;
    auto originBase = base;
    int hitCount = 0;
    while (true) {
      //  ray.timeMax = 1;
        ptr->si = scene.intersect(ray);
        if (!ptr->si)
            break;
        if ((ptr->si->position - originBase.position).length() > initialL)
            break;
        base = ptr->si.value();
        if (ptr->si->object == po.object) {
            IntersectionChain *next = new IntersectionChain();
            ptr->next = next;
            ptr->rayDir = ray.direction;
            ptr = next;
            hitCount++;
        }
        ray = base.spawnRay(pTarget);
    }

    // Randomly choose one of several intersections during BSSRDF sampling
    if (hitCount == 0)
        return {0.0f};
    int selected = clamp((int)(u1 * hitCount), 0, hitCount - 1);
    int tempHitCount = hitCount;
    hitCount -= selected;
    while (selected-- > 0) {
        IntersectionChain *next = chain->next;
        delete chain;
        chain = next;
    }
    *pi = chain->si.value();
    while (hitCount-- > 0) {
        IntersectionChain *next = chain->next;
        delete chain;
        chain = next;
    }
    *pdf = this->pdfSP(*pi) / tempHitCount;
    return this->Sr((pi->position - po.position).length());
}

Spectrum TabelBSSRDF::Sr(double d) const {
   // d = 0.02f;r
  //  return sigmaA/20.f;
    auto sigma_a = sigmaA, sigma_s = sigmaS;
    Spectrum sigma_t = sigma_a + sigma_s, rho;
    for (int c = 0; c < 3; ++c)
        rho[c] = sigma_t[c] != 0 ? (sigma_s[c] / sigma_t[c]) : 0;

    Spectrum Sr(0.f);
    for (int ch = 0; ch < 3; ++ch) {
        // Convert $r$ into unitless optical radius $r_{\roman{optical}}$
        double rOptical = d * sigma_t[ch];

        // Compute spline weights to interpolate BSSRDF on channel _ch_
        int rhoOffset, radiusOffset;
        double rhoWeights[4], radiusWeights[4];
        if (!CatmullRomWeights(table.nRhoSamples, table.rhoSamples.get(),
                               rho[ch], &rhoOffset, rhoWeights) ||
            !CatmullRomWeights(table.nRadiusSamples, table.radiusSamples.get(),
                               rOptical, &radiusOffset, radiusWeights))
            continue;

        // Set BSSRDF value _Sr[ch]_ using tensor spline interpolation
        double sr = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                double weight = rhoWeights[i] * radiusWeights[j];
                if (weight != 0)
                    sr += weight *
                          table.EvalProfile(rhoOffset + i, radiusOffset + j);
            }
        }

        // Cancel marginal PDF factor from tabulated BSSRDF profile
        if (rOptical != 0) sr /= 2 * M_PI * rOptical;
        Sr[ch] = sr;
    }
    // Transform BSSRDF value into world space units
    Sr *= sigma_t * sigma_t;
    return Sr;
}

double TabelBSSRDF::sampleSr(int ch, double u) const {
    auto sigma_a = sigmaA, sigma_s = sigmaS;
    Spectrum sigma_t = sigma_a + sigma_s, rho;
    for (int c = 0; c < 3; ++c)
        rho[c] = sigma_t[c] != 0 ? (sigma_s[c] / sigma_t[c]) : 0;
    if (sigma_t[ch] == 0) return -1;
    auto t = SampleCatmullRom2D(table.nRhoSamples, table.nRadiusSamples,
                                table.rhoSamples.get(), table.radiusSamples.get(),
                                table.profile.get(), table.profileCDF.get(),
                                rho[ch], u) /
             sigma_t[ch];
    return t;
}

Spectrum TabelBSSRDF::pdfSr(double r) const {
    Spectrum pdf(0);
    auto sigma_a = sigmaA, sigma_s = sigmaS;
    Spectrum sigma_t = sigma_a + sigma_s;
    Spectrum rho;
    for (int c = 0; c < 3; ++c)
        rho[c] = sigma_t[c] != 0 ? (sigma_s[c] / sigma_t[c]) : 0;
    for (int ch = 0; ch < 3; ch++) {

        double rOptical = r * sigma_t[ch];

        // Compute spline weights to interpolate BSSRDF density on channel _ch_
        int rhoOffset, radiusOffset;
        double rhoWeights[4], radiusWeights[4];
        if (!CatmullRomWeights(table.nRhoSamples, table.rhoSamples.get(), rho[ch],
                               &rhoOffset, rhoWeights) ||
            !CatmullRomWeights(table.nRadiusSamples, table.radiusSamples.get(),
                               rOptical, &radiusOffset, radiusWeights))
            return 0.f;

        // Return BSSRDF profile density for channel _ch_
        double sr = 0, rhoEff = 0;
        for (int i = 0; i < 4; ++i) {
            if (rhoWeights[i] == 0) continue;
            rhoEff += table.rhoEff[rhoOffset + i] * rhoWeights[i];
            for (int j = 0; j < 4; ++j) {
                if (radiusWeights[j] == 0) continue;
                sr += table.EvalProfile(rhoOffset + i, radiusOffset + j) *
                      rhoWeights[i] * radiusWeights[j];
            }
        }
        if (rOptical != 0) sr /= 2 * M_PI * rOptical;

        pdf[ch] = sr * sigma_t[ch] * sigma_t[ch] / rhoEff;
    }
    return  pdf;
    // Cancel marginal PDF factor from tabulated BSSRDF profile
}

double TabelBSSRDF::maxSr(int ch) const {
    return sampleSr(ch, 0.999);
}

Spectrum SeparableBSSRDF::Sw(const Vec3d &w) const {
    double c = 1 - 2 * FresnelMoment1(1 / eta);
    return Spectrum(1 - Fresnel::dielectricReflectance(eta, CosTheta(w))) / (c * M_PI);
}

SeparableBSSRDF::SeparableBSSRDF(double eta, const Intersection &po) : BSSRDF(eta), po(po),
                                                                       ns(po.geometryNormal),
                                                                       ss(cross(ns, po.geometryTangent)),
                                                                       ts(cross(ss, ns)),
                                                                       adapterMaterial(std::make_shared<BSSRDFAdapterMaterial>(this)) {
    coordinateSystem(ns, ss, ts);
}

TabelBSSRDF::TabelBSSRDF(double eta, Spectrum sigma_a, Spectrum sigma_s, double g, const BSSRDFTable &table,
                         const Intersection &po)
    : SeparableBSSRDF(eta, po), table(table), sigmaA(sigma_a), sigmaS(sigma_s) {
}

double BeamDiffusionMultScattering(double sigmaS, double sigmaA, double g, double eta, double r) {
    // return 0;
    const int nSamples = 100;

    ///forward-scattering light will mostly continue in the same direction
    ///backward-scattering light will mostly scatter back and absorb.
    sigmaS = sigmaS * (1 - g);
    double sigmaT = sigmaA + sigmaS;
    ///reduced albedo
    double rhop = sigmaS / sigmaT;

    ///non-classical diffusion coefficient
    double Dg = (2 * sigmaA + sigmaS) / (3 * sqr(sigmaS + sigmaA));

    ///effective transport coefficient. model the multiple scattering inside the medium
    double sigmaTr = sqrt(sigmaA / Dg);
    double fm1 = FresnelMoment1(eta), fm2 = FresnelMoment2(eta);
    double ze = -2 * Dg * (1 + 3 * fm2) / (1 - 2 * fm1);

    double cPhi = .25f * (1 - 2 * fm1);
    double cE = 0.5f * (1 - 3 * fm2);

    double Ed = 0;
    for (int i = 0; i < nSamples; i++) {
        ///real point source and virtual source depth
        double zr = -std::log(1 - (i + 0.5f) / nSamples) / sigmaT;
        double zv = -zr + 2 * ze;

        double dr = sqrt(sqr(r) + sqr(zr));
        double dv = sqrt(sqr(r) + sqr(zv));

        double phiD = 0.25 / M_PI / Dg * (std::exp(-sigmaTr * dr) / dr - std::exp(-sigmaTr * dv) / dv);
        double EDn = 0.25 / M_PI * (zr * (1 + sigmaTr * dr) * std::exp(-sigmaTr * dr) / (dr * dr * dr) - zv * (1 + sigmaTr * dv) * std::exp(-sigmaTr * dv) / (dv * dv * dv));
        double E = phiD * cPhi + EDn * cE;
        double kappa = 1 - std::exp(-2 * sigmaT * (dr + zr));
        Ed += kappa * rhop * rhop * E;
    }
    return Ed / nSamples;
}

// Media Inline Functions
inline double PhaseHG(double cosTheta, double g) {
    double denom = 1 + g * g + 2 * g * cosTheta;
    return 0.25 / M_PI * (1 - g * g) / (denom * std::sqrt(denom));
}

double BeamDiffusionSingleScattering(double sigmaS, double sigmaA, double g, double eta, double r) {
    //return 0;
    // sigmaA = 0.1 ,sigmaS = 0.1,g=1.0,eta= 1.3,r=0.1;
    double sigma_t = sigmaA + sigmaS, rho = sigmaS / sigma_t;
    double tCrit = r * std::sqrt(eta * eta - 1);
    double Ess = 0;
    const int nSamples = 100;
    for (int i = 0; i < nSamples; ++i) {
        // Evaluate single scattering integrand and add to _Ess_
        double ti = tCrit - std::log(1 - (i + .5f) / nSamples) / sigma_t;

        // Determine length $d$ of connecting segment and $\cos\theta_\roman{o}$
        double d = std::sqrt(r * r + ti * ti);
        double cosThetaO = ti / d;

        // Add contribution of single scattering at depth $t$
        Ess += rho * std::exp(-sigma_t * (d + tCrit)) / (d * d) *
               PhaseHG(cosThetaO, g) * (1 - Fresnel::dielectricReflectance(1 / eta, -cosThetaO)) *
               std::abs(cosThetaO);
        auto temp1 = rho * std::exp(-sigma_t * (d + tCrit)) / (d * d);
        auto temp2 = Fresnel::dielectricReflectance(1 / eta, -cosThetaO);
        int k = 1;
    }
    return Ess / nSamples;
}

void computeBeamDiffusionBSSRDF(double g, double eta, BSSRDFTable *t) {
    t->radiusSamples[0] = 0;
    t->radiusSamples[1] = 2.5e-3f;
    for (int i = 2; i < t->nRadiusSamples; ++i)
        t->radiusSamples[i] = t->radiusSamples[i - 1] * 1.2f;

    // Choose albedo values of the diffusion profile discretization
    for (int i = 0; i < t->nRhoSamples; ++i)
        t->rhoSamples[i] =
            (1 - std::exp(-8 * i / (double)(t->nRhoSamples - 1))) /
            (1 - std::exp(-8));
    for (int i = 0; i < t->nRhoSamples; i++) {
        // Compute the diffusion profile for the _i_th albedo sample

        // Compute scattering profile for chosen albedo $\rho$
        for (int j = 0; j < t->nRadiusSamples; ++j) {
            BeamDiffusionSingleScattering(0.5, 0.5, 0, 1.3, 0.2);
            double rho = t->rhoSamples[i], r = t->radiusSamples[j];
            auto temp = 2 * M_PI * r * (BeamDiffusionMultScattering(rho, 1 - rho, g, eta, r) + BeamDiffusionSingleScattering(rho, 1 - rho, g, eta, r));
            t->profile[i * t->nRadiusSamples + j] = temp;
        }
        t->rhoEff[i] =
            IntegrateCatmullRom(t->nRadiusSamples, t->radiusSamples.get(),
                                &t->profile[i * t->nRadiusSamples],
                                &t->profileCDF[i * t->nRadiusSamples]);
    }
}

BSSRDFTable::BSSRDFTable(int nRhoSamples, int nRadiusSamples) : nRhoSamples(nRhoSamples),
                                                                nRadiusSamples(nRadiusSamples),
                                                                rhoSamples(new double[nRhoSamples]),
                                                                radiusSamples(new double[nRadiusSamples]),
                                                                profile(new double[nRadiusSamples * nRhoSamples]),
                                                                rhoEff(new double[nRhoSamples]),
                                                                profileCDF(new double[nRadiusSamples * nRhoSamples]) {}

void SubsurfaceFromDiffuse(const BSSRDFTable &t, const Spectrum &rhoEff, const Spectrum &mfp, Spectrum *sigma_a,
                           Spectrum *sigma_s) {
    for (int c = 0; c < 3; ++c) {
        double rho = InvertCatmullRom(t.nRhoSamples, t.rhoSamples.get(),
                                      t.rhoEff.get(), rhoEff[c]);
        (*sigma_s)[c] = rho / mfp[c];
        (*sigma_a)[c] = (1 - rho) / mfp[c];
    }
}
