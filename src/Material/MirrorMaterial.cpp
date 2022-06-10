//
// Created by 袁军平 on 2022/6/10.
//

#include "MirrorMaterial.h"


std::shared_ptr<BxDF> MirrorMaterial::getBxDF(Intersection intersect) const
{
    std::shared_ptr<Mirror> bxdf = std::make_shared<Mirror>();
    return bxdf;
}
std::shared_ptr<BSSRDF> MirrorMaterial::getBSSRDF(Intersection intersect) const
{
    return nullptr;
}