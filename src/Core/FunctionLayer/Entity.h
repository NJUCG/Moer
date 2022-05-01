/**
 * @file Entity.h
 * @author orbitchen
 * @brief Objects that can be intersected with ray.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Transform3d.h"
#include "AccelerateNode.h"
#include "Intersection.h"
#include "Ray.h"

class Entity : public Transform3D
{
public:
	Intersection intersect(const Ray &r) const = 0;
};