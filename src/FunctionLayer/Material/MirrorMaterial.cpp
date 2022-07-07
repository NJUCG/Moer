
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
std::shared_ptr<BxDF> MirrorMaterial::getBxDF(Intersection intersect) const
{
    std::shared_ptr<Mirror> bxdf = std::make_shared<Mirror>();
    return bxdf;
}


std::shared_ptr<BSSRDF> MirrorMaterial::getBSSRDF(Intersection intersect) const
{
    return nullptr;
}