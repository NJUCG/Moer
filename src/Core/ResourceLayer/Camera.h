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

#include "../CoreLayer/Geometry.h"
#include "Ray.h"

class Camera
{
public:

	// @brief generate a ray by NDC coord ranges between [-1,1].
	Ray generateRay(Point2f NDC);

	// TODO sampler?

};