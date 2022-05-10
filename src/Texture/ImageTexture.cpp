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

