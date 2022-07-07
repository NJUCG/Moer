/**
 * @file DielectricMaterial.cpp
 * @author Junping Yuan
 * @brief
 * @version 0.1
 * @date 2022/6/11
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "DelectricMaterial.h"
std::shared_ptr<BxDF> DelectricMaterial::getBxDF(Intersection intersect) const {
    return std::make_shared<Dielectric>();
}

std::shared_ptr<BSSRDF> DelectricMaterial::getBSSRDF(Intersection intersect) const {
    return nullptr ;
}
