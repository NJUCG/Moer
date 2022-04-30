/**
 * @file Texture.h
 * @author orbitchen
 * @brief Texture of different types.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../CoreLayer/Geometry.h"
#include "../CoreLayer/Color.h"
#include <cmath>
#include "Intersection.h"
#include "../FunctionLayer/Image.h"

// Tvalue can be float or any specturm type
template <typename Tvalue>
class Texture
{
public:
	virtual Tvalue eval(const Intersection &intersection) const = 0;
};

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

// @brief TextureMapping maps an intersection to texture coordinates of type Tcoord.
template <typename Tcoord>
class TextureMapping
{
protected:
public:
	virtual Tcoord mapping(const Intersection &intersection) const = 0;
};

// @brief TextureFunction maps texture coordinates to texture value.
template <typename Tvalue, typename Tcoord>
class TextureFunction
{
protected:
public:
	virtual Tvalue eval(const Tcoord &coord) const = 0;
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

// ImageTexture as a TextureFunction2D

enum class WrapMode
{
	// todo
};

class ImageSampler
{
protected:
	WrapMode wrapMode;

public:
	virtual void setWrapMode(enum WrapMode _wrapMode)
	{
		wrapMode = _wrapMode;
	}
	// todo: other common parameters

	virtual void loadImage(const std::string &filename) = 0;
	virtual RGB3 sample(const TextureCoord2D &coord) = 0;
	virtual RGB3 texel(const Point2i &coord) = 0;
};

class DirectImageSampler : public ImageSampler
{
protected:
	std::shared_ptr<Image> image;

public:
	virtual void loadImage(const std::string &filename);
	virtual RGB3 sample(const TextureCoord2D &coord);
	virtual RGB3 texel(const Point2i &coord);
};

class ImageTextureFunction2D : public TextureFunction<RGB3, TextureCoord2D>
{
protected:
public:
	ImageTextureFunction2D(const std::string &filename); // using default sampler
	ImageTextureFunction2D(const std::string &filename, std::shared_ptr<ImageSampler> imageSampler);
	virtual RGB3 eval(const TextureCoord2D &coord) const = 0;
};

class ImageTexture : public StdTexture<RGB3, TextureCoord2D>
{
protected:
public:
	ImageTexture(const std::string &filename); // using default sampler
	ImageTexture(const std::string &filename, std::shared_ptr<ImageSampler> imageSampler);
	ImageTexture(const std::string &filename, std::shared_ptr<TextureMapping<TextureCoord2D>> mapping);
	ImageTexture(const std::string &filename, std::shared_ptr<ImageSampler> imageSampler, std::shared_ptr<TextureMapping<TextureCoord2D>> mapping);
};

// * Example: Creating a Image-based Texture using UV coordinates from mesh
// * >  ImageTexture("1.jpg");
// * >  ImageTexture("1.jpg", std::make_shared<DirectSampler>());
// * >  ImageTexture("1.jpg", std::make_shared<UVTextureMapping2D>());
// * >  ImageTexture("1.jpg", std::make_shared<DirectSampler>(), std::make_shared<UVTextureMapping2D>());
// * >  StdTexture(std::make_shared<ImageTextureFunction2D>("1.jpg"));
// * >  StdTexture(std::make_shared<ImageTextureFunction2D>("1.jpg"), std::make_shared<DirectSampler>());

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////
// Various kinds of other TextureFunction (procedural)

class CheckerboardTextureFunction2D : public TextureFunction<float, TextureCoord2D>
{
protected:
public:
	virtual float eval(const TextureCoord2D &coord) const override
	{
		return int(floor(coord.coord.x) + floor(coord.coord.y)) & 1 ? 1.0f : 0.0f;
	}
};

class CheckerboardTextureFunction3D : public TextureFunction<float, TextureCoord3D>
{
protected:
public:
	virtual float eval(const TextureCoord3D &coord) const override
	{
		return int(floor(coord.coord.x) + floor(coord.coord.y) + floor(coord.coord.z)) & 1 ? 1.0f : 0.0f;
	}
};
