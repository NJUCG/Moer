/**
 * @file Ray.cpp
 * @author Zhimin Fan
 * @brief Ray implemention.
 * @version 0.1
 * @date 2022-05-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Ray.h"

Ray::Ray(const Point3d &_origin, const Vec3d &_direction, double _timeMin, double _timeMax) : origin(_origin),
                                                                                              direction(_direction)
{
	timeMin = _timeMin;
	timeMax = _timeMax;
}