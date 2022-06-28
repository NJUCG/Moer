/**
 * @file BoundingBox.h
 * @author orbitchen
 * @brief Bounding box declaration.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Geometry.h"

#include <algorithm>

/*
 * @brief Bounding box 3D base type.
 * @param BaseType the type of the bounding box, int/float/double.
 */
template <typename BaseType>
class BoundingBox3
{
	TPoint3<BaseType> pMin;
	TPoint3<BaseType> pMax;

public:
	BoundingBox3() {};

	BoundingBox3(TPoint3<BaseType> _pMin, TPoint3<BaseType> _pMax)
	{
		pMin = _pMin;
		pMax = _pMax;
	}

	/*
	 * @brief union of two bounding boxes.
	 * @return the smallest bounding box that bounds both b1 and b2.
	 */
	static BoundingBox3 BoundingBoxUnion(const BoundingBox3& b1, const BoundingBox3& b2)
	{
		TPoint3<BaseType> _pMin = (std::min(b1[0], b2[0]), std::min(b1[1], b2[1]), std::min(b1[2], b2[2]));
		TPoint3<BaseType> _pMax = (std::max(b1[0], b2[0]), std::max(b1[1], b2[1]), std::max(b1[2], b2[2]));
		return BoundingBox3(_pMin, _pMax);
	}
	/*
	 * @brief intersect of two bounding boxes.
	 * @return the largest bounding box that bounded by both b1 and b2.
	 */
	static BoundingBox3 BoundingBoxUnionIntersect(const BoundingBox3& b1, const BoundingBox3& b2)
	{
		TPoint3<BaseType> _pMin = (std::max(b1[0], b2[0]), std::max(b1[1], b2[1]), std::max(b1[2], b2[2]));
		TPoint3<BaseType> _pMax = (std::min(b1[0], b2[0]), std::min(b1[1], b2[1]), std::min(b1[2], b2[2]));
		return BoundingBox3(_pMin, _pMax);
	}
};

using BoundingBox3f = BoundingBox3<double>;
