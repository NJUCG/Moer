
/**
 * @file MirrorMaterial.h
 * @author Junping Yuan
 * @brief
 * @version 0.1
 * @date 2022/6/9
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "MirrorMaterial.h"
std::shared_ptr<BxDF> MirrorMaterial::getBxDF(const Intersection & intersect) const
{
    std::shared_ptr<MirrorBxDF> bxdf = std::make_shared<MirrorBxDF>();
    return bxdf;
}


std::shared_ptr<BSSRDF> MirrorMaterial::getBSSRDF(const Intersection & intersect) const
{
    return nullptr;
}