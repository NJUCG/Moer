//
// Created by 袁军平 on 2022/6/7.
//

#include "Matte.h"

std::shared_ptr<BxDF> Matte::getBxDF(Intersection intersect) const
{
    Spectrum color(0.4,0.2,0.3);
    std::shared_ptr<Diffuse> bxdf = std::make_shared<Diffuse>(color);
    return bxdf;
}
std::shared_ptr<BSSRDF> Matte::getBSSRDF(Intersection intersect) const
{
    return nullptr;
}