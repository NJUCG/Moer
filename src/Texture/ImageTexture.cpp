/**
 * @file ImageTexture.cpp
 * @author Zhimin Fan
 * @brief Image texture.
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ImageTexture.h"

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
T DirectImage<T>::eval(const TextureCoord2D &coord)
{
}

template <typename T>
ImageTexture<T>::ImageTexture(const std::string &filename,
                              std::shared_ptr<TextureMapping2D> mapping) : mapping(mapping)
{
}

template <typename T>
ImageTexture<T>::ImageTexture(const std::string &filename,
                              std::shared_ptr<PrefilteredImage<T>> imageSampler,
                              std::shared_ptr<TextureMapping2D> mapping) : mapping(mapping)
{
}

template <typename T>
T ImageTexture<T>::eval(const TextureCoord2D &coord) const
{
    return RGB3(coord.coord.x, coord.coord.y, 0.0);
}