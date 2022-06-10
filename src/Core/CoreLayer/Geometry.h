/**
 * @file Geometry.h
 * @author zcx
 * @brief The geometry part of whole program, including vector, point and normal.
 * @version 0.1
 * @date 2022-04-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../../Geometry/normal.h"
#include "../../Geometry/point.h"
#include "../../Geometry/vector.h"


// vector.h
template <typename T>
struct TVector2;
template <typename T>
struct TVector3;
using Vec2f = TVector2<float>;
using Vec2d = TVector2<double>;
using Vec2i = TVector2<int>;
using Vec3f = TVector3<float>;
using Vec3d = TVector3<double>;
using Vec3i = TVector3<int>;
// point.h
template <typename T>
struct TPoint2;
template <typename T>
struct TPoint3;
using Point2f = TPoint2<float>;
using Point2d = TPoint2<double>;
using Point2i = TPoint2<int>;
using Point3f = TPoint3<float>;
using Point3d = TPoint3<double>;
using Point3i = TPoint3<int>;
// normal.h
struct Normal3d;
