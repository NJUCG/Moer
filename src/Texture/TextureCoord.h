/**
 * @file TextureCoord.h
 * @author Zhimin Fan
 * @brief Texture coord.
 * @version 0.1
 * @date 2022-05-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../Core/CoreLayer/Geometry.h"

// @brief Struct for texture coord C and dC/dx, dC/dy.
template <typename Tpos, typename Tvec>
struct TextureCoord
{
	Tpos coord;
	Tvec dcdx;
	Tvec dcdy;
};

typedef TextureCoord<Point2f, Vec2f> TextureCoord2D;
typedef TextureCoord<Point3f, Vec3f> TextureCoord3D;