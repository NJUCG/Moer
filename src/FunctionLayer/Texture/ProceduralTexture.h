/**
 * @file ProceduralTexture.h
 * @author Zhimin Fan
 * @brief Procedural texture.
 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once
#include "Texture.h"
#include "TextureMapping.h"
/// \brief 2D checkboard texture
/// \ingroup Texture
template<class T>
class Checkerboard2D : public StdTexture<T, TextureCoord2D>
{
protected:
    T onColor;
    T offColor;
    int resU;
    int resV;
public:
    Checkerboard2D(T onValue,T offValue,int resU = 16 ,int resV = 8);
    using StdTexture<T, TextureCoord2D>::eval;
    virtual T eval(const TextureCoord2D &coord) const override;
};

template<class T>
Checkerboard2D<T>::Checkerboard2D(T onValue, T offValue, int resU, int resV)
    :onColor(onValue),offColor(offValue),resU(resU),resV(resV), StdTexture<T, TextureCoord2D>(std::make_shared<UVTextureMapping2D>())
{

}



template<class T>
T Checkerboard2D<T>::eval(const TextureCoord2D &coord) const {
    int x = coord.coord.x * resU;
    int y = coord.coord.y * resV;
    bool on = (x ^ y) & 1;
    return on ? onColor : offColor;
}

/// \brief 3D checkboard texture
/// \ingroup Texture
class Checkerboard3D : public StdTexture<double, TextureCoord3D>
{
protected:
public:
    Checkerboard3D();
    using StdTexture::eval;
    virtual double eval(const TextureCoord3D &coord) const override;
};
