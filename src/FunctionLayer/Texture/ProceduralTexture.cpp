/**
 * @file ProceduralTexture.cpp
 * @author Zhimin Fan
 * @brief Procedural texture.
 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "ProceduralTexture.h"
#include "TextureMapping.h"

Checkerboard2D::Checkerboard2D() : StdTexture(std::make_shared<UVTextureMapping2D>())
{
}

Checkerboard3D::Checkerboard3D() : StdTexture(std::make_shared<NaturalTextureMapping3D>())
{
}

double Checkerboard2D::eval(const TextureCoord2D &coord) const
{
    return int(floor(coord.coord.x) + floor(coord.coord.y)) & 1 ? 1.0f : 0.0f;
}

double Checkerboard3D::eval(const TextureCoord3D &coord) const
{
    return int(floor(coord.coord.x) + floor(coord.coord.y) + floor(coord.coord.z)) & 1 ? 1.0f : 0.0f;
}