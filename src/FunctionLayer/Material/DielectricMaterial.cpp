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
#include "FunctionLayer/Texture/TextureFactory.h"
DielectricMaterial::DielectricMaterial(const Json &json): Material(json) {
    ior = getOptional(json, "ior", 1.33); //water
    albedoR = TextureFactory::LoadTexture<>(json,"albedo_reflection",RGB3(1,1,1));
    albedoT = TextureFactory::LoadTexture<>(json,"albedo_transmission",RGB3(1,1,1));
    if(json.contains("roughness"))
        roughness = TextureFactory::LoadTexture<double>(json["roughness"]);
    if(json.contains("u_roughness"))
        uRoughness =  TextureFactory::LoadTexture<double>(json["u_roughness"]);
    if(json.contains("v_roughness"))
        vRoughness =  TextureFactory::LoadTexture<double>(json["v_roughness"]);
    if(roughness || uRoughness || vRoughness){
        distrib = LoadDistributionFromJson(json);
    }
    twoSideShading = false;
}

std::shared_ptr<BxDF> DielectricMaterial::getBxDF(const Intersection & intersect) const  {
    Spectrum specularT = albedoT->eval(intersect);
    Spectrum specularR = albedoR->eval(intersect);
    if( roughness || uRoughness || vRoughness){
        double  uRough = uRoughness?uRoughness->eval(intersect):roughness->eval(intersect);
        double  vRough = uRoughness?uRoughness->eval(intersect):roughness->eval(intersect);
        return std::make_shared <RoughDielectricBxDF>(ior,specularR,specularT,uRough,vRough,distrib);
    }
    else {
        return  std::make_shared<DielectricBxDF>(ior,specularR,specularT);
    }
}

std::shared_ptr<BSSRDF> DielectricMaterial::getBSSRDF(const Intersection & intersect) const{
    return nullptr ;
}
