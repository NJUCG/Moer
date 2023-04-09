#pragma once

#include <cmath>
#include <numeric>
#include "CoreLayer/Geometry/Frame.h"
#include "data.h"

namespace PathGuiding::vmm {

// the number of components in a vMF mixture
constexpr int NComponents = 32;

using SampleIterator = std::vector<SampleData>::iterator;

// TODO approximate the exp?

// TODO adaptive split and merge?

// for vMF sampling and pdf evaluation
// see Jakob https://www.mitsuba-renderer.org/~wenzel/files/vmf.pdf
class VMFKernel {
public:

    const Vec3d mu;
    const double kappa{};
    const double alpha{};

    inline VMFKernel & operator=(const VMFKernel & kernel) {
        const_cast<Vec3d &>(mu) = kernel.mu;
        const_cast<double &>(kappa) = kernel.kappa;
        const_cast<double &>(alpha) = kernel.alpha;
        eMin2Kappa = kernel.eMin2Kappa;
        pdfFactor = kernel.pdfFactor;
        return *this;
    }

    inline void setMu(const Vec3d & v) {
        const_cast<Vec3d &>(mu) = v;
    }

    inline void setKappa(double k) {
        const_cast<double &>(kappa) = k;
        refreshCache();
    }

    inline void setAlpha(double a) {
        const_cast<double &>(alpha) = a;
    }

    [[nodiscard]]
    inline Vec3d sample(const Vec2d & rn) const {
        // sample in the local coordinate
        double phi = 2. * M_PI * rn.y;
        double sinPhi = std::sin(phi);
        double cosPhi = std::cos(phi);

        double value = rn.x + (1. - rn.x) * eMin2Kappa;
        double cosTheta = clamp(1. + std::log(value) / kappa, -1., 1.);
        double sinTheta = std::sqrt(1. - cosTheta * cosTheta);

        Vec3d omega(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);

        // rotate to the world coordinate
        return Frame({mu.x, mu.y, mu.z}).toWorld(omega);
    }

    [[nodiscard]]
    inline double pdf(const Vec3d & v) const {
        double t = dot(mu, v) - 1.;
        double e = std::exp(kappa * t);
        return pdfFactor * e;
    }

private:

    friend class VMFMixture;
    friend class ParallaxAwareVMM;

    double eMin2Kappa{};
    double pdfFactor{};

    inline VMFKernel() = default;

    // clamp value to be in [a, b]
    inline static double clamp(double value, double a, double b) {
        return std::min(std::max(value, a), b);
    }

    // the exp() operation is very expensive, so we cache it
    inline void refreshCache() {
        eMin2Kappa = std::exp(-2. * kappa);
        double de = 2 * M_PI * (1. - eMin2Kappa);
        pdfFactor = kappa / de;
    }
};

class VMFMixture {
public:

    [[nodiscard]]
    inline Vec3d sample(const Vec2d & rn) const {
        // pick a component
        int k = 0;
        double accAlphaSum = 0;
        for (; k < NComponents - 1; ++k) {
            if (rn.x < accAlphaSum + kernels[k].alpha) {
                break;
            }
            accAlphaSum += kernels[k].alpha;
        }

        // sample the picked component
        Vec2d reusedSample((rn.x - accAlphaSum) / kernels[k].alpha, rn.y);
        return kernels[k].sample(reusedSample);
    }

    [[nodiscard]]
    inline double pdf(const Vec3d & w) const {
        double value = 0;
        for (const auto & kernel: kernels) {
            value += kernel.alpha * kernel.pdf(w);
        }
        return value;
    }

private:

    friend class ParallaxAwareVMM;

    VMFKernel kernels[NComponents];

};

class ParallaxAwareVMM : public VMFMixture {
public:

    inline ParallaxAwareVMM() {
        for (int k = 0; k < NComponents; ++k) {
            // initialize mu with spherical Fibonacci point set,
            // which is uniformly distributed on the unit sphere.
            // see Marques et al. "Spherical Fibonacci Point Sets for Illumination Integrals" for more details
            double phi = 2. * k * M_PI * 0.6180339887498949;
            double sinPhi = std::sin(phi);
            double cosPhi = std::cos(phi);
            double cosTheta = 1 - (double) (2 * k + 1) / NComponents;
            double sinTheta = std::sqrt(1. - cosTheta * cosTheta);
            Vec3d mu(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);

            kernels[k].setMu(mu);
            kernels[k].setAlpha(1. / NComponents);
            kernels[k].setKappa(5);
            meanCosine[k] = 0;
            distances[k] = std::numeric_limits<double>::infinity();
            distanceWeightSums[k] = 0;
        }

        currentPosition = Vec3d(0);
        sampleWeightSum = 0;
        batchIndex = 0;
    }

    inline void update(const Vec3d & newPosition, SampleIterator begin, SampleIterator end) {
        warpTo(newPosition);

        // relocate the samples
        for (auto iter = begin; iter != end; ++iter) {
            Vec3d origin = iter->position + iter->direction * iter->distance;
            Vec3d po = origin - newPosition;
            double t = po.length();
            if (t > 0) {
                iter->direction = po / t;
                iter->distance = t;
            }
        }

        updateKernels(begin, end);
        updateDistances(begin, end);
    }

    inline void getWarped(VMFMixture & out, const Vec3d & newPosition) const {
        Vec3d newToCurrent = currentPosition - newPosition;
        for (int k = 0; k < NComponents; ++k) {
            Vec3d po = kernels[k].mu * distances[k] + newToCurrent;
            double t = po.length();

            out.kernels[k] = kernels[k];
            if (std::isfinite(distances[k]) && t > 0) {
                out.kernels[k].setMu(po * (1 / t));
            }
        }
    }

    inline ParallaxAwareVMM * split() {
        sampleWeightSum *= 0.25;
        for (auto & distanceWeightSum : distanceWeightSums) {
            distanceWeightSum *= 0.25f;
        }

        return new ParallaxAwareVMM(*this);
    }

private:

    double meanCosine[NComponents]{};
    double distances[NComponents]{};
    double distanceWeightSums[NComponents]{};
    Vec3d currentPosition;
    double sampleWeightSum{};
    double batchIndex{};

    // TODO use a more accurate approximation within the specified range?
    inline static double meanCosineToKappa(double meanCosine) {
        double kappa = meanCosine * (3. - meanCosine * meanCosine) / (1. - meanCosine * meanCosine);
        return std::max(std::min(kappa, 1e+4), 1e-2);
    }

    // move the central model to the new reference position
    inline void warpTo(const Vec3d & newPosition) {
        Vec3d newToCurrent = currentPosition - newPosition;
        for (int k = 0; k < NComponents; ++k) {
            Vec3d po = kernels[k].mu * distances[k] + newToCurrent;
            double t = po.length();
            if (std::isfinite(distances[k]) && t > 0) {
                distances[k] = t;
                kernels[k].setMu(po / t);
            }
        }
        currentPosition = newPosition;
    }

    // this implementation uses a variant of weighted incremental EM,
    // see Liang and Klein. "Online EM for Unsupervised Models" for a brief pseudocode
    void updateKernels(SampleIterator begin, SampleIterator end) {
        // compute previous sufficient statistics
        double lastGammaWeightSums[NComponents];
        Vec3d lastGammaWeightSampleSums[NComponents];
        for (int k = 0; k < NComponents; ++k) {
            lastGammaWeightSums[k] = sampleWeightSum * kernels[k].alpha;
            lastGammaWeightSampleSums[k] = kernels[k].mu * meanCosine[k] * lastGammaWeightSums[k];
        }

        // compute sample weights
        double batchSampleWeightSum = std::accumulate(begin, end, 0.,
            [](double sum, const SampleData & sample) -> double {
                return sum + sample.radiance / sample.pdf;
            }
        );

        batchIndex += 1;
        double movingWeight = 1. / batchIndex;
        sampleWeightSum = lerp(sampleWeightSum, batchSampleWeightSum, movingWeight);

        double partialPdfs[NComponents];
        double batchGammaWeightSums[NComponents];
        Vec3d batchGammaWeightSampleSums[NComponents];

        const int maxIterations = 128;
        const double threshold = 5e-3;

        // the while loop still converges in an online manner,
        // and is actually more stable in the context of our application
        int iteration = 0;
        double lastLogLikelihood = 0;
        while (iteration < maxIterations) {
            double logLikelihood = 0;
            std::fill(batchGammaWeightSums, batchGammaWeightSums + NComponents, 0);
            std::fill(batchGammaWeightSampleSums, batchGammaWeightSampleSums + NComponents, Vec3d(0));

            // compute batch sufficient statistics
            int i = 0;
            for (auto iter = begin; iter != end; ++iter) {
                double pdf = 0;
                for (int k = 0; k < NComponents; ++k) {
                    partialPdfs[k] = kernels[k].alpha * kernels[k].pdf(iter->direction);
                    pdf += partialPdfs[k];
                }

                // in case of running out of precision (i.e. pdf = 0)
                pdf = std::max(pdf, std::numeric_limits<double>::min());

                double weight = iter->radiance / iter->pdf;
                for (int k = 0; k < NComponents; ++k) {
                    double gammaIK = partialPdfs[k] / pdf;
                    batchGammaWeightSums[k] += gammaIK * weight;
                    batchGammaWeightSampleSums[k] += gammaIK * weight * iter->direction;
                }

                logLikelihood += weight * std::log(pdf);
                i += 1;
            }

            // update parameters
            for (int k = 0; k < NComponents; ++k) {
                double gammaWeightSum = lerp(lastGammaWeightSums[k], batchGammaWeightSums[k], movingWeight);
                Vec3d gammaWeightSampleSum = lerp(lastGammaWeightSampleSums[k], batchGammaWeightSampleSums[k], movingWeight);
                double rLength = gammaWeightSampleSum.length();

                if (gammaWeightSum > 0 && rLength > 0) {
                    kernels[k].setAlpha(gammaWeightSum / sampleWeightSum);
                    kernels[k].setMu(gammaWeightSampleSum / rLength);
                    meanCosine[k] = std::min(rLength / gammaWeightSum, 0.9999);
                    kernels[k].setKappa(meanCosineToKappa(meanCosine[k]));
                }
            }

            if (iteration >= 1) {
                // TODO the log likelihood can sometimes drop, may be due to the approximation of kappa?
                if ((logLikelihood - lastLogLikelihood) / std::abs(lastLogLikelihood) < threshold) {
                    break;
                }
            }

            iteration += 1;
            lastLogLikelihood = logLikelihood;
        }
        // TODO better strategy to handle components with extreme values?
    }

    void updateDistances(SampleIterator begin, SampleIterator end) {
        double componentPdfs[NComponents];
        double batchWeightedDistanceSums[NComponents];
        double batchDistanceWeightSums[NComponents];

        std::fill(batchWeightedDistanceSums, batchWeightedDistanceSums + NComponents, 0);
        std::fill(batchDistanceWeightSums, batchDistanceWeightSums + NComponents, 0);

        for (auto iter = begin; iter != end; ++iter) {
            double weight = iter->radiance / iter->pdf;

            double pdf = 0;
            for (int k = 0; k < NComponents; ++k) {
                componentPdfs[k] = kernels[k].pdf(iter->direction);
                pdf += kernels[k].alpha * componentPdfs[k];
            }

            for (int k = 0; k < NComponents; ++k) {
                double gammaIK = kernels[k].alpha * componentPdfs[k] / pdf;
                double distanceWeight = weight * gammaIK * componentPdfs[k];
                batchWeightedDistanceSums[k] += distanceWeight / iter->distance;
                batchDistanceWeightSums[k] += distanceWeight;
            }
        }

        double movingWeight = 1. / batchIndex;
        for (int k = 0; k < NComponents; ++k) {
            double weightedDistanceSum = distanceWeightSums[k] / distances[k];
            double newWeightSum = lerp(distanceWeightSums[k], batchDistanceWeightSums[k], movingWeight);
            double newWeightedDistanceSum = lerp(weightedDistanceSum, batchWeightedDistanceSums[k], movingWeight);

            if (batchDistanceWeightSums[k] > 0 && batchWeightedDistanceSums[k] > 0) {
                distances[k] = newWeightSum / newWeightedDistanceSum;
                distanceWeightSums[k] = newWeightSum;
            }
        }
    }
};

}
