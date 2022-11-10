/**
 * @file TextureMapping.h
 * @author Zhimin Fan
 * @brief Texture mapping.
 * @version 0.1
 * @date 2022-05-03
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <cmath>
#include "CoreLayer/ColorSpace/Color.h"
#include "Texture.h"
#include "FunctionLayer/Intersection.h"

// Various kinds of TextureMapping

class UVTextureMapping2D : public TextureMapping<TextureCoord2D>
{
protected:
    // todo: add a affine transform
public:
    virtual TextureCoord2D mapping(const Intersection &intersection) const override;
};

class NaturalTextureMapping3D : public TextureMapping<TextureCoord3D>
{
protected:
    // todo: add a affine transform
public:
    virtual TextureCoord3D mapping(const Intersection &intersection) const override;
};