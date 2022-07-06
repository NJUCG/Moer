/**
 * @file ImageTexture.cpp
 * @author Zhimin Fan
 * @brief Image texture.
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "ImageTexture.h"

template <>
RGB3 DirectImage<RGB3>::eval(const TextureCoord2D &coord)
{
    Point2d uv = coord.coord;
    // todo: apply wrap mode
    uv.x = std::max(0.0, std::min(0.99999, uv.x)) * image->getResolution().x;
    uv.y = std::max(0.0, std::min(0.99999, uv.y)) * image->getResolution().y;
    Point2i xy(uv.x, uv.y);
    return image->getRGBColorAt(xy);
}

template <>
Spectrum ImageTexture<Spectrum, RGB3>::eval(const TextureCoord2D &coord) const
{
    return imageSampler->eval(coord).toSpectrum();
}