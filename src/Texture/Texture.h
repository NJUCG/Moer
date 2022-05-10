/**
 * @file Texture.h
 * @author Zhimin Fan
 * @brief Texture of different types.
 * @version 0.1
 * @date 2022-05-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <cmath>
#include "../Core/FunctionLayer/Texture.h"
#include "../Core/FunctionLayer/Intersection.h"
#include "TextureCoord.h"
#include "TextureMapping.h"
#include "TextureFunction.h"
#include "ImageTexture.h"

template <typename Tvalue>
class ConstantTexture : public Texture<Tvalue>
{
protected:
    Tvalue value;

public:
    ConstantTexture(const Tvalue &value);
    virtual Tvalue eval(const Intersection &intersection) const;
};

template <typename Tvalue>
class MixTexture : public Texture<Tvalue>
{
protected:
    std::shared_ptr<Texture<Tvalue>> srcA;
    std::shared_ptr<Texture<Tvalue>> srcB;
    std::shared_ptr<Texture<float>> factor;

public:
    MixTexture(std::shared_ptr<Texture<Tvalue>> srcA,
               std::shared_ptr<Texture<Tvalue>> srcB,
               std::shared_ptr<Texture<float>> factor);

    virtual Tvalue eval(const Intersection &intersection) const;
};

// @brief StdTexture refers to a texture which consists of a TextureMapping and a TextureFunction.
template <typename Tvalue, typename Tcoord>
class StdTexture : public Texture<Tvalue>
{
protected:
    std::shared_ptr<TextureMapping<Tcoord>> map;
    std::shared_ptr<TextureFunction<Tvalue, Tcoord>> func;

public:
    StdTexture(std::shared_ptr<TextureFunction<Tvalue, Tcoord>> func,
               std::shared_ptr<TextureMapping<Tcoord>> map);

    StdTexture(std::shared_ptr<TextureFunction<Tvalue, Tcoord>> func);

    virtual Tvalue eval(const Intersection &intersection) const;
};
