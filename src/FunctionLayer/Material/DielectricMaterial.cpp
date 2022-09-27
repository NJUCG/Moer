/**
 * @file DielectricMaterial.cpp
 * @author Junping Yuan
 * @brief
 * @version 0.1
 * @date 2022/6/11
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "DielectricMaterial.h"
#include "FunctionLayer/Medium/Beerslaw.h"
DielectricMaterial::DielectricMaterial(const Json &json) {
    //! If bumpmap, not specularTransmission
    type = EMaterialType::SpecularTransmission;
    double _ior = getOptional(json, "ior", 1.0);
    Vec3d _albedo = getOptional(json, "albedo", Vec3d{1, 1, 1});
    ior = std::make_shared<ConstantTexture<double>>(_ior);
    albedo = std::make_shared<ConstantTexture<Spectrum>>(RGB3(_albedo.x, _albedo.y, _albedo.z).toSpectrum());
    
}

std::shared_ptr<BxDF> DielectricMaterial::getBxDF(const Intersection & intersect) const  {
    return std::make_shared<DielectricBxDF>(ior->eval(intersect), albedo->eval(intersect));
}

std::shared_ptr<BSSRDF> DielectricMaterial::getBSSRDF(const Intersection & intersect) const{
    return nullptr ;
}
