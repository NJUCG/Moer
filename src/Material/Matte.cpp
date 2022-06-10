//
// Created by 袁军平 on 2022/6/7.
//

#include "Matte.h"

Matte::Matte(std::shared_ptr<Texture<Spectrum>> a) : albedo(a)
{
}

std::shared_ptr<BxDF> Matte::getBxDF(Intersection intersect) const
{
    Spectrum color = albedo->eval(intersect);
    std::shared_ptr<Diffuse> bxdf = std::make_shared<Diffuse>(color);
    return bxdf;
}
std::shared_ptr<BSSRDF> Matte::getBSSRDF(Intersection intersect) const
{
    return nullptr;
}