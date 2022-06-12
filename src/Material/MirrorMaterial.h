/**
 * @file
 * @author Junping Yuan
 * @brief
 * @version 0.1
 * @date 2022/6/9
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/Mirror.h"
#include "Core/FunctionLayer/Intersection.h"

#pragma  once
class MirrorMaterial  : public  Material{


public:
    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;
    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;
};
