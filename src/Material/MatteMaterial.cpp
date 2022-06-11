//
// Created by 袁军平 on 2022/6/7.
//

#include "MatteMaterial.h"

std::shared_ptr<BxDF> MatteMaterial::getBxDF(Intersection intersect) const
{
    auto color = kd->eval(intersect);
    std::shared_ptr<Diffuse> bxdf = std::make_shared<Diffuse>(color);
    return bxdf;
}
std::shared_ptr<BSSRDF> MatteMaterial::getBSSRDF(Intersection intersect) const
{
    return nullptr;
}