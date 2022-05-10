/**
 * @file ImageTexture.h
 * @author Zhimin Fan
 * @brief Image texture.
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
#include "TextureMapping.h"
#include "TextureFunction.h"

enum class WrapMode
{
    // todo
};

template <typename T>
class PrefilteredImage
{
protected:
    WrapMode wrapMode;

public:
    virtual void setWrapMode(enum WrapMode _wrapMode);
    // todo: other common parameters

    virtual void loadImage(const std::string &filename) = 0;
    virtual T sample(const TextureCoord2D &coord) = 0;
    virtual T texel(const Point2i &coord) = 0;
};

template <typename T>
class DirectPrefilteredImage : public PrefilteredImage<T>
{
protected:
    std::shared_ptr<Image> image;

public:
    virtual void loadImage(const std::string &filename);
    virtual T sample(const TextureCoord2D &coord);
    virtual T texel(const Point2i &coord);
};

template <typename T>
class ImageTextureFunction2D : public TextureFunction<T, TextureCoord2D>
{
protected:
public:
    ImageTextureFunction2D(const std::string &filename); // using default sampler
    ImageTextureFunction2D(const std::string &filename, std::shared_ptr<PrefilteredImage<T>> imageSampler);
    virtual T eval(const TextureCoord2D &coord) const = 0;
};

template <typename Tvalue, typename Tcoord>
class StdTexture;

// @brief Convenient class for image-based StdTexture
template <typename T>
class ImageTexture : public StdTexture<T, TextureCoord2D>
{
protected:
public:
    ImageTexture(const std::string &filename); // using default sampler
    ImageTexture(const std::string &filename, std::shared_ptr<PrefilteredImage<T>> imageSampler);
    ImageTexture(const std::string &filename, std::shared_ptr<TextureMapping<TextureCoord2D>> mapping);
    ImageTexture(const std::string &filename, std::shared_ptr<PrefilteredImage<T>> imageSampler, std::shared_ptr<TextureMapping<TextureCoord2D>> mapping);
};

// * Example: Creating a Image-based Color Texture using UV coordinates from mesh
// * >  ImageTexture<RGB3>("1.jpg");
// * >  ImageTexture<RGB3>("1.jpg", std::make_shared<DirectSampler>());
// * >  ImageTexture<RGB3>("1.jpg", std::make_shared<UVTextureMapping2D>());
// * >  ImageTexture<RGB3>("1.jpg", std::make_shared<DirectSampler>(), std::make_shared<UVTextureMapping2D>());
// * >  StdTexture<RGB3,TextureCoord2D>(std::make_shared<ImageTextureFunction2D>("1.jpg"));
// * >  StdTexture<RGB3,TextureCoord2D>(std::make_shared<ImageTextureFunction2D>("1.jpg"), std::make_shared<DirectSampler>());

// * Example: Create a Image-based Normal Map (wip)
// * since normal cannot be directly interpolated, you need to provide T with some compact NDF type
// * The NDF must provide convertor from RGB3 and some accessors for shadings (depends on implementation of material)