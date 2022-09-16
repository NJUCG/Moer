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

#include "DielectricMaterial.h"
std::shared_ptr<BxDF> DielectricMaterial::getBxDF(const Intersection & intersect) const  {
    return std::make_shared<DielectricBxDF>(ior->eval(intersect));
}

std::shared_ptr<BSSRDF> DielectricMaterial::getBSSRDF(const Intersection & intersect) const{
    return nullptr ;
}
