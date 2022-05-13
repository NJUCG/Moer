/**
 * @file ProceduralTexture.h
 * @author Zhimin Fan
 * @brief Procedural texture.
 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Texture.h"

class Checkerboard2D : public StdTexture<double, TextureCoord2D>
{
protected:
public:
    // todo: constructor
    virtual double eval(const TextureCoord2D &coord) const override;
};

class Checkerboard3D : public StdTexture<double, TextureCoord3D>
{
protected:
public:
    // todo: constructor
    virtual double eval(const TextureCoord3D &coord) const override;
};

