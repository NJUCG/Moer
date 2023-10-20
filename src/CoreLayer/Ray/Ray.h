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
#include <cfloat>
#include "CoreLayer/Geometry/Geometry.h"


/// \brief ray
struct Ray
{
	Point3d origin;		///< The origin of the ray
	Vec3d direction;	///< The direction of the ray

	// ray differential
	bool hasDifferential; 
	Point3d origin_x, origin_y;
	Vec3d direction_x, direction_y;

	double timeMin;		///< The time when the shutter open
	double timeMax;		///< The time when the shutter close
	Point3d at(double t) const ;

	bool withinTime(double time);

	Ray(const Point3d &_origin, const Vec3d &_direction, double _timeMin = .0f, double _timeMax = DBL_MAX);
};

/// \brief ray differential
struct RayDifferential : public Ray
{
	Point3d origin_x, origin_y;
	Vec3d direction_x, direction_y;
};