/**
 * @file fwd.h
 * @author zcx
 * @brief forward declaration
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

template <typename T> struct TVector2;
template <typename T> struct TVector3;
using vec2f = TVector2<float>;
using vec2d = TVector2<double>;
using vec2i = TVector2<int>;
using vec3f = TVector3<float>;
using vec3d = TVector3<double>;
using vec3i = TVector3<int>;
template <typename T> struct TPoint2;
template <typename T> struct TPoint3;
using point2f = TPoint2<float>;
using point2d = TPoint2<double>;
using point2i = TPoint2<int>;
using point3f = TPoint3<float>;
using point3d = TPoint3<double>;
using point3i = TPoint3<int>;
