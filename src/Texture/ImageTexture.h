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

// @brief Convenient class for image-based StdTexture
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
