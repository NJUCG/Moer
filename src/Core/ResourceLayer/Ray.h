/**
 * @file Ray.h
 * @author orbitchen
 * @brief Ray representation.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../CoreLayer/Geometry.h"

struct Ray
{
	Point3f origin;
	Vec3f direction;

	double timeMin;
	double timeMax;

	Point3f at(double t);

	bool withinTime(double time);

	Ray(const Point3f& _origin, const Point3f& _direction,double _timeMin=.0f,double _timeMax=.0f);
};