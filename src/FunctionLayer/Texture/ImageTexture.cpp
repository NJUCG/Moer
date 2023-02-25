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

    //apply linear lerp

    int _w = image->getResolution().x;
    int _h = image->getResolution().y;

    float u = uv.x * image->getResolution().x;
    float v = uv.y * image->getResolution().y;
    bool linear = true;
    if (linear) {
        u -= 0.5f;
        v -= 0.5f;
    }
    int iu0 = u < 0.0f ? -int(-u) - 1 : int(u);
    int iv0 = v < 0.0f ? -int(-v) - 1 : int(v);
    int iu1 = iu0 + 1;
    int iv1 = iv0 + 1;

    u-=iu0;
    v-=iv0;
    iu0 = clamp(iu0, 0, _w - 1);
    iu1 = clamp(iu1, 0, _w - 1);
    iv0 = clamp(iv0, 0, _h - 1);
    iv1 = clamp(iv1, 0, _h - 1);

    return  lerp(
            image->getRGBColorAt(Point2i(iu0,iv0)),
            image->getRGBColorAt(Point2i(iu1,iv0)),
            image->getRGBColorAt(Point2i(iu0,iv1)),
            image->getRGBColorAt(Point2i(iu1,iv1)),
            u,
            v
    );


    // todo: apply wrap mode
//    uv.x = std::max(0.0, std::min(0.99999, uv.x)) * image->getResolution().x;
//    uv.y = std::max(0.0, std::min(0.99999, uv.y)) * image->getResolution().y;
//    Point2i xy(uv.x, uv.y);
//    return image->getRGBColorAt(xy);
}

template<>
Spectrum ImageTexture<Spectrum, RGB3>::eval(const TextureCoord2D &coord) const
{
    return Spectrum(imageSampler->eval(coord));
}

template<>
double ImageTexture<double, RGB3>::eval(const TextureCoord2D &coord) const
{
    return imageSampler->eval(coord)[0];
}

template<>
RGB3 ImageTexture<RGB3, RGB3>::eval(const TextureCoord2D &coord) const
{
    return imageSampler->eval(coord);
}