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

#include "Texture.h"

/// \brief 2D checkboard texture
/// \ingroup Texture
class Checkerboard2D : public StdTexture<double, TextureCoord2D>
{
protected:
public:
    Checkerboard2D();
    using StdTexture::eval;
    virtual double eval(const TextureCoord2D &coord) const override;
};

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
