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

//#include "../core.h"
#include <memory>
#include "CoreLayer/Geometry/Geometry.h"
#include "FunctionLayer/Medium/Medium.h"

struct Ray
{
	Point3d origin;
	Vec3d direction;

	double timeMin;
	double timeMax;

	std::shared_ptr<Medium> medium;

	Point3d at(double t);

	bool withinTime(double time);

	Ray(const Point3d &_origin, const Vec3d &_direction, double _timeMin = .0f, double _timeMax = .0f);
};

struct RayDifferential : public Ray
{
	Point3d origin_x, origin_y;
	Vec3d direction_x, direction_y;
};