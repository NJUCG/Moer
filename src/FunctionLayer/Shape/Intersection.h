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

#include "CoreLayer/Geometry/Frame.h"
#include "CoreLayer/Geometry/Geometry.h"
#include "FunctionLayer/Material/Material.h"
#include "FunctionLayer/Medium/Medium.h"
#include "Entity.h"

#include <memory>

struct Intersection
{
	Point3d position;
	Normal3d geometryNormal;
	Normal3d geometryTangent;
	Normal3d geometryBitangent;
	Point2d uv;

	// shadingFrame
	Frame shFrame;

	Point3d dpdu, dpdv;
	Normal3d dndu, dndv;

	// std::shared_ptr<Entity> object;
	const Entity *object;

	std::shared_ptr<Material> material;
	std::shared_ptr<Medium> mediumInside;
	std::shared_ptr<Medium> mediumOutside;

	Vec3d toLocal(const Vec3d &d) const
	{
		return shFrame.toLocal(d);
	}

	Vec3d toWorld(const Vec3d &d) const
	{
		return shFrame.toWorld(d);
	}
};