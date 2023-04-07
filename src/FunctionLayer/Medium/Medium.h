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

struct MediumSampleRecord {
    double marchLength;
	double pdf;
    Point3d scatterPoint;
    Vec3d wi;

    Spectrum tr;
    Spectrum sigmaA;
    Spectrum sigmaS;
};


class Medium {
public:

	/**
	 * @brief Sample a distance, the ray will transport in media without collision until reach the distance
	 * @return 
	 * - true, if the ray will endure a collision in medium
	 * - false, if the ray will pass through the media without collision
	 */

	Medium(std::shared_ptr<PhaseFunction> phase) : mPhase(phase) { }

	virtual bool sampleDistance(MediumSampleRecord *mRec, 
								const Ray &ray,
								const Intersection &itsOpt,
								Point2d sample) const = 0;

	virtual Spectrum evalTransmittance (Point3d from, Point3d dest) const = 0;

	auto evalPhase(Vec3d wo, Vec3d wi, Point3d scatterPoint) const {
		return mPhase->evalPhase(wo, wi, scatterPoint);
	}

	auto samplePhase(Vec3d wo, Point3d scatterPoint, Point2d sample) const {
		return mPhase->samplePhase(wo, scatterPoint, sample);
	}

protected:

	std::shared_ptr<PhaseFunction> mPhase;

};