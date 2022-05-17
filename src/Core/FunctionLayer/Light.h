/**
 * @file Intersection.h
 * @author orbitchen
 * @brief Light representation.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../CoreLayer/Geometry.h"
#include "../CoreLayer/Color.h"
#include "Ray.h"

struct LightSampleResult
{
	// @brief Spectrum from light.
	Spectrum s;

	// @brief Point on an object where receives light.
	Point3d src;

	// @brief Point on a light where emits light.
	Point3d dst;

	Ray ray;

	Normal3d dstNormal;
	double pdfPos;
	double pdfDir;
	Point2d uv;

	// @brief FALSE for area and volume light, TRUE for point and etc
	bool isDeltaPos;
	bool isDeltaDir;
	bool isInf;
};

class Light
{
public:
	virtual LightSampleResult eval(const Ray &ray) = 0;
	virtual LightSampleResult sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time) = 0;
	virtual LightSampleResult sampleDirect(const Intersection& its, const Point2d &sample, float time) = 0;
};