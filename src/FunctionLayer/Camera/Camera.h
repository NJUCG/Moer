/**
 * @file Camera.h
 * @author orbitchen
 * @brief Different cameras. Need to be investigated.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Ray/Ray.h"

class Camera
{
public:

	//// @brief generate a ray by NDC coord ranges between [-1,1].
	// change NDC unit square
	virtual Ray generateRay(Point2d NDC) const = 0;

	// TODO sampler?
	virtual Ray generateRay(Point2d NDC, Point2d sample) const = 0;
};