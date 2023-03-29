/**
 * @file ImageTexture.h
 * @author Zhimin Fan
 * @brief Image texture.
 * @version 0.1
 * @date 2022-05-03
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "CoreLayer/ColorSpace/Color.h"
#include "ResourceLayer/File/Image.h"
#include "ResourceLayer/ResourceManager.h"
#include "FunctionLayer/Intersection.h"
#include "Texture.h"
#include "TextureMapping.h"
#include "FastMath.h"

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
    virtual WrapMode getWrapMode();
    // todo: other common parameters

    virtual T eval(const TextureCoord2D &coord) = 0;
    virtual T texel(const Point2i &coord) = 0;
    virtual void loadImage(const std::string &filename) = 0;

    virtual int getImageWidth() const = 0;
    virtual int getImageHeight() const =0;
};

template <typename T>
class DirectImage : public PrefilteredImage<T>
{
protected:
    std::shared_ptr<Image> image;

public:
    virtual void loadImage(const std::string &filename) override;
    virtual T eval(const TextureCoord2D &coord) override;
    virtual T texel(const Point2i &coord) override;

    int getImageWidth( ) const override;
    int getImageHeight( ) const override;
};

template <typename T>
class LinearMIPMap : public PrefilteredImage<T>
{
protected:
    std::shared_ptr<Image> image;

public:
    virtual void loadImage(const std::string &filename) override;
    virtual T eval(const TextureCoord2D &coord);
    virtual T texel(const Point2i &coord);
};

/// \brief Image-based texture
/// \ingroup Texture
template <typename Treturn, typename Tmemory>
class ImageTexture : public StdTexture<Treturn, TextureCoord2D>
{
protected:
    std::shared_ptr<PrefilteredImage<Tmemory>> imageSampler;

public:
    ImageTexture(const std::string &filename,
                 std::shared_ptr<TextureMapping2D> mapping = std::make_shared<UVTextureMapping2D>()); // using default sampler
    ImageTexture(const std::string &filename,
                 std::shared_ptr<PrefilteredImage<Tmemory>> imageSampler,
                 std::shared_ptr<TextureMapping2D> mapping = std::make_shared<UVTextureMapping2D>());
    virtual Treturn eval(const TextureCoord2D &coord) const override;
    int getWidth() const {return imageSampler->getImageWidth();}
    int getHeight() const {return imageSampler->getImageHeight();}
    std::vector<double> sphericalWeighs();
};

// * Example: Creating a Image-based Color Texture using UV coordinates from mesh
// * >  ImageTexture<RGB3>("1.jpg");
// * >  ImageTexture<RGB3>("1.jpg", std::make_shared<DirectSampler>());
// * >  ImageTexture<RGB3>("1.jpg", std::make_shared<UVTextureMapping2D>());
// * >  ImageTexture<RGB3>("1.jpg", std::make_shared<DirectSampler>(), std::make_shared<UVTextureMapping2D>());

// * Example: Create a Image-based Normal Map (wip)
// * since normal cannot be directly interpolated, you need to provide T with some compact NDF type
// * The NDF must provide convertor from RGB3 and some accessors for shadings (depends on implementation of material)

template <typename T>
void PrefilteredImage<T>::setWrapMode(enum WrapMode _wrapMode)
{
    wrapMode = _wrapMode;
}

template <typename T>
WrapMode PrefilteredImage<T>::getWrapMode()
{
    return wrapMode;
}

template <typename T>
void DirectImage<T>::loadImage(const std::string &filename)
{
    image = ImageManager::getInstance()->getImage(filename);
}

template <typename T>
T DirectImage<T>::texel(const Point2i &coord)
{
    return image->getRGBColorAt(coord);
}

template < typename T >
int DirectImage < T >::getImageWidth( ) const {
    return image->getWidth();
}

template < typename T >
int DirectImage < T >::getImageHeight( ) const {
    return image->getHeight();
}

template <typename Treturn, typename Tmemory>
ImageTexture<Treturn, Tmemory>::ImageTexture(const std::string &filename,
                                             std::shared_ptr<TextureMapping2D> mapping) : StdTexture<Treturn, TextureCoord2D>(mapping)
{
    imageSampler = std::make_shared<DirectImage<Tmemory>>();
    imageSampler->loadImage(filename);
}



template <typename Treturn, typename Tmemory>
ImageTexture<Treturn, Tmemory>::ImageTexture(const std::string &filename,
                                             std::shared_ptr<PrefilteredImage<Tmemory>> imageSampler,
                                             std::shared_ptr<TextureMapping2D> mapping) : imageSampler(imageSampler), StdTexture<Treturn, TextureCoord2D>(mapping)
{
    this->imageSampler->loadImage(filename);
}

template < typename Treturn, typename Tmemory >
std::vector < double > ImageTexture < Treturn, Tmemory >::sphericalWeighs( ) {
    int _w = getWidth();
    int _h = getHeight();
    std::vector<double> weights(_w*_h);
    for (int y = 0, idx = 0; y < _h; ++y) {
        double rowWeight = 1.0;
        rowWeight *= fm::sin((y*M_PI)/_h);
        for (int x = 0; x < _w; ++x, ++idx)
        {
            Treturn val = imageSampler->texel(Point2i(x,y));
            weights[idx] = val.luminance() * rowWeight;
        }
    }
    return std::move(weights);
}
