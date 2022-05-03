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
    ConstantTexture(const Tvalue &value) : value(value) {}
    virtual Tvalue eval(const Intersection &intersection) const
    {
        return value;
    }
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
               std::shared_ptr<Texture<float>> factor) : srcA(srcA), srcB(srcB), factor(factor)
    {
    }

    virtual Tvalue eval(const Intersection &intersection) const
    {
        float alpha = factor->eval(intersection);
        return srcA->eval(intersection) * alpha + srcB->eval(intersection) * (1 - alpha);
    }
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
               std::shared_ptr<TextureMapping<Tcoord>> map) : map(map), func(func)
    {
    }

    StdTexture(std::shared_ptr<TextureFunction<Tvalue, Tcoord>> func) : func(func)
    {
        // todo: use default mapping for 2D and 3D
    }

    virtual Tvalue eval(const Intersection &intersection) const
    {
        Tcoord coord = map->mapping(intersection);
        return func->eval(coord);
    }
};
