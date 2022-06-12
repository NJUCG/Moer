//
// Created by 袁军平 on 2022/6/11.
//

#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/Dielectric.h"
#include "Core/FunctionLayer/Intersection.h"
#include "Core/FunctionLayer/Texture.h"
#pragma  once

class DelectricMaterial : public  Material{\

public:
    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;

    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;

    DelectricMaterial(const  std::shared_ptr<Texture<float>> & m_intIDR, const  std::shared_ptr<Texture<float>> & m_extIDR) : m_intIDR(m_intIDR)
    {

    }

    DelectricMaterial(){

    }

private:
    std::shared_ptr<Texture<float>> m_intIDR;
    std::shared_ptr<Texture<float>> m_extIDR;

};


