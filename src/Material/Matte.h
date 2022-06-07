//
// Created by 袁军平 on 2022/6/7.
//


#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/Diffuse.h"



#pragma  once
class Matte     : public  Material{
private:
    Spectrum albedo;
public:
    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;
    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;

};


