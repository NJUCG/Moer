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
#include "../Core/FunctionLayer/Intersection.h"
#include "../CoreLayer/Color.h"
#include "Ray.h"

struct LightSampleResult
{
	// @brief Spectrum from light.
	Spectrum s;

	// @brief Point on an object where receives light if exists.
	Point3d src;

	// @brief Point on a light where emits light if exists.
	Point3d dst;

	// @brief Direction from src to dst.
	Vec3d wi;

	Normal3d dstNormal;
	Point2d uv;

	// @brief Total pdf.
	double pdf;
	// @brief Positional PDF if exists.
	double pdfPos;
	// @brief Directional PDF if exists.
	double pdfDir;

	// @brief FALSE for area and volume light, TRUE for point and etc
	bool isDeltaPos;
	// @brief TRUE for distant light
	bool isDeltaDir;
};

class Light
{
public:
	virtual LightSampleResult evalEnvironment(const Ray &ray) = 0;
	virtual LightSampleResult eval(const Intersection &its, const Vec3d &d) = 0;
	virtual LightSampleResult sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time) = 0;
	virtual LightSampleResult sampleDirect(const Intersection &its, const Point2d &sample, float time) = 0;
};