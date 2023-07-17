/**
 * @file MatteMaterial.cpp
 * @author Junping Yuan
 * @brief
 * @version 0.1
 * @date 2022/6/7
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "MatteMaterial.h"
#include "FunctionLayer/Material/BxDF/LambertainBxDF.h"
#include "FunctionLayer/Texture/Texture.h"
#include "FunctionLayer/Texture/TextureFactory.h"

std::shared_ptr<BxDF> MatteMaterial::getBxDF(const Intersection & intersect) const
{
    Spectrum color = albedo->eval(intersect);
    std::shared_ptr<LambertainBxDF> bxdf = std::make_shared<LambertainBxDF>(color);
    return bxdf;
}

std::shared_ptr<BSSRDF> MatteMaterial::getBSSRDF(const Intersection & intersect) const
{
    return nullptr;
}

MatteMaterial::MatteMaterial(const Json & json): Material(json) {
    type = EMaterialType::Diffuse;
    albedo = TextureFactory::LoadTexture<RGB3>(json,"albedo",RGB3(1,1,1));
}

MatteMaterial::MatteMaterial() {
    albedo = TextureFactory::LoadConstantTexture(RGB3(0.5,0.5,0.5));
}
