/**
 * @file BxDF.h
 * @author orbitchen
 * @brief BxDF, including BRDF and BTDF.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../CoreLayer/Color.h"
#include "../CoreLayer/Geometry.h"

struct BxDFSampleResult
{
	Spectrum s;
	Vec3f directionIn;
	double pdf;
	// todo: enum flags
	bool isSpecular;
};

// @brief BxDF. out == rays from/to camera, in == rays from/to objects/lights.
class BxDF
{

public:
	virtual Spectrum f(const Vec3f &out, const Vec3f &in) const = 0;

	virtual Vec3f sampleWi(const Vec3f &out) const = 0;

	virtual double pdf(const Vec3f &out, const Vec3f &in) const = 0;

	virtual BxDFSampleResult sample(const Vec3f &out) const = 0;

	virtual bool isSpecular() const = 0;
};