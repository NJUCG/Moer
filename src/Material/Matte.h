//
// Created by 袁军平 on 2022/6/7.
//

#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/Diffuse.h"
#include "../Texture/Texture.h"

#pragma once
class Matte : public Material
{
private:
    std::shared_ptr<Texture<Spectrum>> albedo;

public:
    Matte(std::shared_ptr<Texture<Spectrum>> a = std::make_shared<ConstantTexture<Spectrum>>(Spectrum(0.5)));
    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;
    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;
};
