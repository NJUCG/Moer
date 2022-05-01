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
	Spectrum s;
	Point3f src;
	Point3f dst;
	Point3f dstNormal;
	double pdf;
	Point2f uv;
};

class Light
{
public:
	
	LightSampleResult sampleAll(const Ray& r) const = 0;
};