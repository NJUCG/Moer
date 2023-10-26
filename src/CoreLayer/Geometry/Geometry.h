/**
 * @file Geometry.h
 * @author Chenxi Zhou
 * @brief All the forward declarations of geometry part
 * @version 0.1
 * @date 2022-04-22
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "Normal.h"
#include "Point.h"
#include "Vector.h"

/// \defgroup Geometry 
/// \todo Cause all the real number will be represented as double, maybe delete the float version  

// Vector.h
template <typename T>
struct TVector2;
template <typename T>
struct TVector3;
using Vec2f = TVector2<float>;
using Vec2d = TVector2<double>;
using Vec2i = TVector2<int>;
using Vec3f = TVector3<float>;
using Vec3d = TVector3<double>;
using Vec4d = TVector4<double>;
using Vec3i = TVector3<int>;
// Point.h
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
// Normal.h
struct Normal3d;
