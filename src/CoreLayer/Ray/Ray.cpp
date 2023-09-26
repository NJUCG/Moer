/**
 * @file Ray.cpp
 * @author Zhimin Fan
 * @brief Ray implemention.
 * @version 0.1
 * @date 2022-05-09
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "Ray.h"

Ray::Ray(const Point3d &_origin, const Vec3d &_direction, double _timeMin, double _timeMax) : origin(_origin),
                                                                                              direction(_direction)
{
    hasDifferential = false;    
	timeMin = _timeMin;
	timeMax = _timeMax;
}

Point3d Ray::at(double t) const {
    return origin + direction * t;
}
