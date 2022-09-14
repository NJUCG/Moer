/**
 * @file BoundingBox.h
 * @author orbitchen
 * @brief Bounding box declaration.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */
#pragma once

#include "Geometry.h"
#include "CoreLayer/Ray/Ray.h"
#include <optional>
#include <algorithm>
#include <limits>

/**
 * @ingroup Geometry
 * @brief Axis-aligned bounding box 3D base type, the box edges are mutually perpendicular and aligned to axes. 
 * @param BaseType the type of the bounding box, int/float/double.
 * @todo Name "AABB3" might better 
 */
template <typename BaseType>
class BoundingBox3
{
public:
	TPoint3<BaseType> pMin;			
	TPoint3<BaseType> pMax;

	BoundingBox3() {
		pMin[0] = pMin[1] = pMin[2] = std::numeric_limits<BaseType>::max();
		pMax[0] = pMax[1] = pMax[2] = std::numeric_limits<BaseType>::min();
	};

	BoundingBox3(TPoint3<BaseType> _p)
	{
		pMin = pMax = _p;
	}
	BoundingBox3(TPoint3<BaseType> _pMin, TPoint3<BaseType> _pMax)
	{
		pMin = _pMin;
		pMax = _pMax;
	}

	/**
	 * \brief Intersect of ray and bounding box
	 * \return A pointer to the pair (t0, t1)
	 */
	std::optional<Point2d> Intersection(const Ray& r) {
		double t0 = 0, t1 = DBL_MAX;
		for (int i = 0; i < 3; i++) {
			double inv = 1.0 / r.direction[i];
			double tNear = (pMin[i] - r.origin[i]) * inv;
			double tFar = (pMax[i] - r.origin[i]) * inv;
			if (tNear > tFar) std::swap(tNear, tFar);
			if (t0 < tNear) t0 = tNear;
			if (t1 > tFar) t1 = tFar;
		}
		if (t0 > t1) return std::nullopt;
		return std::make_optional(Point2d(t0, t1));
	}

	/**
	 * \brief Returns the surface area of the bounding box
	 */
	double SurfaceArea() {
		double x = pMax[0] - pMin[0];
		double y = pMax[1] - pMin[1];
		double z = pMax[2] - pMin[2];
		return 2 * (x * y + x * z + y * z);
	}
};

/**
* \brief Union of two bounding boxes.
* \return The smallest bounding box that bounds both b1 and b2.
*/
template <typename BaseType>
static BoundingBox3<BaseType> BoundingBoxUnion(const BoundingBox3<BaseType>& b1, 
											   const BoundingBox3<BaseType>& b2)
{
	TPoint3<BaseType> _pMin(std::min(b1.pMin[0], b2.pMin[0]), std::min(b1.pMin[1], b2.pMin[1]), std::min(b1.pMin[2], b2.pMin[2]));
	TPoint3<BaseType> _pMax(std::max(b1.pMax[0], b2.pMax[0]), std::max(b1.pMax[1], b2.pMax[1]), std::max(b1.pMax[2], b2.pMax[2]));
	return BoundingBox3(_pMin, _pMax);
}
/**
 * \brief Overlap of two bounding boxes.
 * \return The largest bounding box that bounded by both b1 and b2.
 * \todo The function name "BoundingBoxOverlap" might better
 */
template <typename BaseType>
static BoundingBox3<BaseType> BoundingBoxUnionIntersect(const BoundingBox3<BaseType>& b1, 
														const BoundingBox3<BaseType>& b2)
{
	TPoint3<BaseType> _pMin(std::max(b1.pMin[0], b2.pMin[0]), std::max(b1.pMin[1], b2.pMin[1]), std::max(b1.pMin[2], b2.pMin[2]));
	TPoint3<BaseType> _pMax(std::min(b1.pMax[0], b2.pMax[0]), std::min(b1.pMax[1], b2.pMax[1]), std::min(b1.pMax[2], b2.pMax[2]));
	return BoundingBox3(_pMin, _pMax);
}

template <typename BaseType>
static BoundingBox3<BaseType> BoundingBoxPointUnion(const BoundingBox3<BaseType>& b1,
                                                    const TPoint3<BaseType>& p)
{
    TPoint3<BaseType> _pMin(std::min(b1.pMin[0],p[0]), std::min(b1.pMin[1],p[1]), std::min(b1.pMin[2],p[2]));
    TPoint3<BaseType> _pMax(std::max(b1.pMax[0],p[0]), std::max(b1.pMax[1],p[1]), std::max(b1.pMax[2],p[3]));
    return BoundingBox3(_pMin, _pMax);
}

using BoundingBox3f = BoundingBox3<double>;
