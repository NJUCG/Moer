/**
 @file Texture.h
 * @author Zhimin Fan
 * @brief Texture of different types.
 * @version 0.1
 * @date 2022-05-03
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <cmath>
#include "FunctionLayer/Intersection.h"
#include "CoreLayer/Geometry/Geometry.h"

// Tvalue can be float or any specturm type
/// \defgroup Texture

/// \brief Base class for all textures
/// \ingroup Texture
template <typename Tvalue>
class Texture
{
public:
	virtual Tvalue eval(const Intersection &intersection) const = 0;
};


// @brief Struct for texture coord C and dC/dx, dC/dy.
template <typename Tpos, typename Tvec>
struct TextureCoord
{
    TextureCoord(const Tpos & _coord) : coord(_coord){}
    TextureCoord() = default;
    Tpos coord;
    Tvec dcdx;
    Tvec dcdy;
};

typedef TextureCoord<Point2d, Vec2d> TextureCoord2D;
typedef TextureCoord<Point3d, Vec3d> TextureCoord3D;

/// @brief TextureMapping maps an intersection to texture coordinates of type Tcoord.
template <typename Tcoord>
class TextureMapping
{
protected:
public:
    virtual Tcoord mapping(const Intersection &intersection) const = 0;
};

typedef TextureMapping<TextureCoord2D> TextureMapping2D;
typedef TextureMapping<TextureCoord3D> TextureMapping3D;

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
    std::shared_ptr<Texture<double>> factor;

public:
    MixTexture(std::shared_ptr<Texture<Tvalue>> srcA,
               std::shared_ptr<Texture<Tvalue>> srcB,
               std::shared_ptr<Texture<double>> factor);

    virtual Tvalue eval(const Intersection &intersection) const;
};

/// @brief StdTexture refers to textures that needs a texture mapping to generate Tcoord from Intersection
/// \ingroup Texture
template <typename Tvalue, typename Tcoord>
class StdTexture : public Texture<Tvalue>
{
protected:
    std::shared_ptr<TextureMapping<Tcoord>> mapping;

public:
    StdTexture();
    StdTexture(std::shared_ptr<TextureMapping<Tcoord>> mapping);
    /// @brief This function just redirects the query to eval(coord) using member TextureMapping. Derived should NOT overwrite this.
    virtual Tvalue eval(const Intersection &intersection) const final;

    /// @brief Eval texture value at given texture coord. (Derived needs to implement this)
    virtual Tvalue eval(const Tcoord &coord) const = 0;
};

template <typename Tvalue>
ConstantTexture<Tvalue>::ConstantTexture(const Tvalue &value) : value(value)
{
}

template <typename Tvalue>
Tvalue ConstantTexture<Tvalue>::eval(const Intersection &intersection) const
{
    return value;
}

template <typename Tvalue>
MixTexture<Tvalue>::MixTexture(std::shared_ptr<Texture<Tvalue>> srcA,
                               std::shared_ptr<Texture<Tvalue>> srcB,
                               std::shared_ptr<Texture<double>> factor) : srcA(srcA), srcB(srcB), factor(factor)
{
}

template <typename Tvalue>
Tvalue MixTexture<Tvalue>::eval(const Intersection &intersection) const
{
    double alpha = factor->eval(intersection);
    return srcA->eval(intersection) * alpha + srcB->eval(intersection) * (1 - alpha);
}

//template <typename Tvalue, typename Tcoord>
//StdTexture<Tvalue, Tcoord>::StdTexture()
//{
//    mapping = std::make_shared<UVTextureMapping2D>>();
//}

template <typename Tvalue, typename Tcoord>
StdTexture<Tvalue, Tcoord>::StdTexture(std::shared_ptr<TextureMapping<Tcoord>> mapping): mapping(mapping)
{
}

template <typename Tvalue, typename Tcoord>
Tvalue StdTexture<Tvalue, Tcoord>::eval(const Intersection &intersection) const
{
    return eval(mapping->mapping(intersection));
}