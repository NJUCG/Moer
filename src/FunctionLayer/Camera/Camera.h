/**
 * @file Camera.h
 * @author orbitchen
 * @brief Different cameras. Need to be investigated.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Ray/Ray.h"

struct CameraSample {
	Point2d xy;
	double time;
	Point2d lens;
};

class Camera
{
public:

	virtual Ray generateRay(const Point2i &filmResolution, 
							const Point2i &pixelPosition, 
							const CameraSample &sample) const = 0;
};