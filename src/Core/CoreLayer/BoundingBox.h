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

	BoundingBox3();

	BoundingBox3(TPoint3<BaseType> _pMin, TPoint3<BaseType> _pMax);

	/*
	* @brief union of two bounding boxes.
	* @return the smallest bounding box that bounds both b1 and b2.
	*/
	BoundingBox3 BoundingBoxUnion(const BoundingBox3& b1, const BoundingBox3& b2){
		// TODO
	}

	/*
	* @brief intersect of two bounding boxes.
	* @return the largest bounding box that bounded by both b1 and b2.
	*/
	BoundingBox3 BoundingBoxUnionIntersect(const BoundingBox3& b1, const BoundingBox3& b2) {
		// TODO
	}
};

using BoundingBox3f = BoundingBox3<double>;
