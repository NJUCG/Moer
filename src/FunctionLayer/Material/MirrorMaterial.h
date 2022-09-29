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

#include "Material.h"
#include "FunctionLayer/Material/BxDF/MirrorBxDF.h"
#include "FunctionLayer/Intersection.h"

#pragma  once
class MirrorMaterial  : public  Material{

public:
    std::shared_ptr<BxDF> getBxDF(const Intersection & intersect) const  override;
    std::shared_ptr<BSSRDF> getBSSRDF(const Intersection & intersect) const override;
};
