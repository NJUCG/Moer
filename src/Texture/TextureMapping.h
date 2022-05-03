/**
 * @file TextureMapping.h
 * @author Zhimin Fan
 * @brief Texture mapping.
 * @version 0.1
 * @date 2022-05-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../Core/CoreLayer/Color.h"
#include <cmath>
#include "../Core/FunctionLayer/Texture.h"
#include "../Core/FunctionLayer/Intersection.h"
#include "TextureCoord.h"

// @brief TextureMapping maps an intersection to texture coordinates of type Tcoord.
template <typename Tcoord>
class TextureMapping
{
protected:
public:
    virtual Tcoord mapping(const Intersection &intersection) const = 0;
};

// Various kinds of TextureMapping

class UVTextureMapping2D : public TextureMapping<TextureCoord2D>
{
protected:
    // todo: add a affine transform
public:
    virtual TextureCoord2D mapping(const Intersection &intersection) const override
    {
        TextureCoord2D answer;
        answer.coord = intersection.uv;
        // todo: eval dctdx and dctdy
        return answer;
    }
};

class NaturalTextureMapping3D : public TextureMapping<TextureCoord3D>
{
protected:
    // todo: add a affine transform
public:
    virtual TextureCoord3D mapping(const Intersection &intersection) const override
    {
        TextureCoord3D answer;
        answer.coord = intersection.position;
        // todo: eval dctdx and dctdy
        return answer;
    }
};