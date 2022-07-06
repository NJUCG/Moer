/**
 * @file TextureMapping.cpp
 * @author Zhimin Fan
 * @brief Texture mapping.
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "TextureMapping.h"

TextureCoord2D UVTextureMapping2D::mapping(const Intersection &intersection) const
{
    TextureCoord2D answer;
    answer.coord = intersection.uv;
    // todo: eval dctdx and dctdy
    return answer;
}

TextureCoord3D NaturalTextureMapping3D::mapping(const Intersection &intersection) const
{
    TextureCoord3D answer;
    answer.coord = intersection.position;
    // todo: eval dctdx and dctdy
    return answer;
}