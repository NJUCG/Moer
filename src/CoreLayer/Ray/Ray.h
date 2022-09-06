/**
 * @file Ray.h
 * @author orbitchen
 * @brief Ray representation.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */
#pragma once

#include <memory>
#include "CoreLayer/Geometry/Geometry.h"
#include "FunctionLayer/Medium/Medium.h"
#include <cfloat>

/// \brief ray
struct Ray
{
	Point3d origin;		///< The origin of the ray
	Vec3d direction;	///< The direction of the ray

	double timeMin;		///< The time when the shutter open
	double timeMax;		///< The time when the shutter close

	std::shared_ptr<Medium> medium;		///< The medium

	Point3d at(double t);

	bool withinTime(double time);

	Ray(const Point3d &_origin, const Vec3d &_direction, double _timeMin = .0f, double _timeMax = DBL_MAX);
};

/// \brief ray differential
struct RayDifferential : public Ray
{
	Point3d origin_x, origin_y;
	Vec3d direction_x, direction_y;
};