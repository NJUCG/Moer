//
// Created by 袁军平 on 2022/6/7.
//


#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/Diffuse.h"
#include "Core/FunctionLayer/Intersection.h"
#include "Core/FunctionLayer/Texture.h"
#pragma  once
class MatteMaterial     : public  Material{
private:
    std::shared_ptr<Texture<Spectrum>> kd ;
public:
    MatteMaterial(const std::shared_ptr<Texture<Spectrum>> & kd )  : kd(kd){}

    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;

    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;

};


