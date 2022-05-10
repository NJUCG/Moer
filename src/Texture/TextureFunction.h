/**
 * @file TextureFunction.h
 * @author Zhimin Fan
 * @brief Texture function.
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
#include "../Core/ResourceLayer/Image.h"
#include "TextureCoord.h"

// @brief TextureFunction maps texture coordinates to texture value.
template <typename Tvalue, typename Tcoord>
class TextureFunction
{
protected:
public:
    virtual Tvalue eval(const Tcoord &coord) const = 0;
};

// Various kinds of other TextureFunction (procedural)

class CheckerboardTextureFunction2D : public TextureFunction<float, TextureCoord2D>
{
protected:
public:
    // todo: constructor
    virtual float eval(const TextureCoord2D &coord) const override;
};

class CheckerboardTextureFunction3D : public TextureFunction<float, TextureCoord3D>
{
protected:
public:
    // todo: constructor
    virtual float eval(const TextureCoord3D &coord) const override;
};
