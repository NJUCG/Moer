/**
 * @file
 * @author Junping Yuan
 * @brief
 * @version 0.1
 * @date 2022/6/11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma  once
#include "Material.h"
#include "FunctionLayer/Material/BxDF/Dielectric.h"
#include "FunctionLayer/Intersection.h"
#include "FunctionLayer/Texture/Texture.h"

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


