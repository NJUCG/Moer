/**
 * @file Scene.h
 * @author orbitchen
 * @brief Scene representation. Handle ray intersection.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Entity.h"
#include "Accelerator.h"
#include "Ray.h"

class Scene:
	public Bvh
{
public:

	intersect(const Ray& r) const;

};