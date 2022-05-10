/**
 * @file Intersection.h
 * @author orbitchen
 * @brief Intersection information on an object.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../CoreLayer/Geometry.h"
#include "Material.h"
#include "Medium.h"
#include "Entity.h"

#include <memory>

struct Intersection
{
	Point3f position;
	Normal3f geometryNormal;
	Point2f uv;

	Point3f dpdu, dpdv;
	Normal3f dndu, dndv;

	std::shared_ptr<Entity> object;
	std::shared_ptr<Material> material;
	std::shared_ptr<Medium> mediumInside;
	std::shared_ptr<Medium> mediumOutside;
};