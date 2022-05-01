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
	Vec3f directionOut;
	double pdf;
	bool isSpecular;
};

class BxDF
{

public:

	Spectrum sampleSpectrum(const Vec3f& in, const Vec3f& out) const = 0;

	Vec3f sampleDirection(const Vec3f& out) const = 0;

	double samplePdf(const Vec3f& in, const Vec3f& out) const = 0;

	BxDFSampleResult sampleAll(const Vec3f& out) const = 0;

	bool isSpecular() const = 0;

};