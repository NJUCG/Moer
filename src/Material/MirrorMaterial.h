//
// Created by 袁军平 on 2022/6/10.
//



#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/Mirror.h"
#include "Core/FunctionLayer/Intersection.h"

#pragma  once
class MirrorMaterial  : public  Material{

public:
    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;
    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;

};


