/**
 * @file Medium.h
 * @author orbitchen
 * @brief Medium.
 * 	- edit by zcx, 2022-9-22
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */
#pragma once

#include "Phase.h"
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/ColorSpace/Color.h"
#include "CoreLayer/Ray/Ray.h"
#include "FunctionLayer/Intersection.h"

#include "FunctionLayer/GaussianProcess/GaussianProcess.h"

#include <optional>

struct MediumState {
    // mainly for gpis medium which need the sampler
    Sampler &sampler;
#if defined(ENABLE_GPISMEDIUM)
    GPRealization realization;
#endif
    void reset() {
#if defined(ENABLE_GPISMEDIUM)
        realization.reset();
#endif
    }
};

struct MediumSampleRecord {
    double marchLength;
    double pdf;
    Point3d scatterPoint;
    Vec3d wi;

    Spectrum tr;
    Spectrum sigmaA;
    Spectrum sigmaS;

    Spectrum emission;
    Vec3d aniso;
    bool needAniso;

    MediumState *mediumState;
};

class Medium {
public:
    /**
     * @brief Sample a distance, the ray will transport in media without collision until reach the distance
     * @return
     * - true, if the ray will endure a collision in medium
     * - false, if the ray will pass through the media without collision
     */

    Medium(std::shared_ptr<PhaseFunction> phase) : mPhase(phase) {}

    virtual bool sampleDistance(MediumSampleRecord *mRec,
                                const Ray &ray,
                                const Intersection &its,
                                Point2d sample) const = 0;

    bool sampleDistanceSafe(MediumSampleRecord *mRec,
                            const Ray &ray,
                            const std::optional<Intersection> &itsOpt,
                            Point2d sample) {

        // this should not happen actually since our medium should be in a bounding box.
        // but with testing,we found that sometimes ray can not in and out the medium and hit the boundary correctly because of the accuracy
        // we have already chosen a small 'eps' to avoid the problem,but we don't know if that will happen again.
        // provided a safe version so that the program won't crash
        if (!itsOpt) {
            mRec->tr = 1.;
            mRec->pdf = 1.;
            return false;
        }
        return sampleDistance(mRec, ray, itsOpt.value(), sample);
    }

    virtual Spectrum evalTransmittance(Point3d from, Point3d dest) const = 0;

    // interface for medium those who need state.
    virtual Spectrum evalTransmittance2(Point3d from, Point3d dest, MediumState *mediumState) const {
        return evalTransmittance(from, dest);
    }

    auto evalPhase(Vec3d wo, Vec3d wi, Point3d scatterPoint) const {
        return mPhase->evalPhase(wo, wi, scatterPoint);
    }

    auto samplePhase(Vec3d wo, Point3d scatterPoint, Point2d sample) const {
        return mPhase->samplePhase(wo, scatterPoint, sample);
    }

protected:
    std::shared_ptr<PhaseFunction> mPhase;
};