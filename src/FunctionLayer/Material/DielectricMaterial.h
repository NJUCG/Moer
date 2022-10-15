/**
 * @file DielectricMaterial.h
 * @author JunPing Yuan
 * @brief
 * @version 0.1
 * @date 2022/6/11
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma  once
#include "Material.h"
#include "FunctionLayer/Material/BxDF/DielectricBxDF.h"
#include "FunctionLayer/Intersection.h"
#include "FunctionLayer/Texture/Texture.h"

class DielectricMaterial : public  Material{

public:
    std::shared_ptr<BxDF> getBxDF(const Intersection & intersect) const override;

    std::shared_ptr<BSSRDF> getBSSRDF(const Intersection & intersect) const override;

    DielectricMaterial(const Json &json);
private:
    std::shared_ptr<Texture<double>> roughness = nullptr, uRoughness = nullptr, vRoughness = nullptr;
    double ior;
    std::shared_ptr<MicrofacetDistribution> distrib;
    std::shared_ptr<Texture<RGB3>> albedoR,albedoT;

};


