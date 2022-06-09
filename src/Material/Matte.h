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
    Matte(Spectrum a = Spectrum(0.5));
    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;
    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;

};


