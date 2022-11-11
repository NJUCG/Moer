/**
 * @file Intersection.h
 * @author orbitchen
 * @brief Intersection information on an object.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <memory>
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Geometry/Frame.h"
#include "FunctionLayer/Material/Material.h"
#include "FunctionLayer/Shape/Entity.h"

//* Add t in intersection, by zcx 8-22
struct Intersection
{
	double t;

	Point3d position;
	Normal3d geometryNormal;
	Normal3d geometryTangent;
	Normal3d geometryBitangent;
	Point2d uv;

	// shadingFrame
	Frame shFrame;

	Vec3d dpdu, dpdv;
	Normal3d dndu, dndv;

	// std::shared_ptr<Entity> object;
	const Entity *object;

	std::shared_ptr<Material> material;

	Vec3d toLocal(const Vec3d &d) const
	{
		return shFrame.toLocal(d);
	}

	Vec3d toWorld(const Vec3d &d) const
	{
		return shFrame.toWorld(d);
	}
};
