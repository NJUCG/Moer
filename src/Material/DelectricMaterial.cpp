//
// Created by 袁军平 on 2022/6/11.
//

#include "DelectricMaterial.h"


std::shared_ptr<BxDF> DelectricMaterial::getBxDF(Intersection intersect) const {
    return std::make_shared<Dielectric>();
}

std::shared_ptr<BSSRDF> DelectricMaterial::getBSSRDF(Intersection intersect) const {
    return std::shared_ptr<BSSRDF>();
}
