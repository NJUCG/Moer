#include "ConductorMaterial.h"
#include "FunctionLayer/Texture/Texture.h"
#include "BxDF/ConductorBxDF.h"
#include "BxDF/GlintBxDF.h"
#include "BxDF/MicrofacetDistribution.h"
#include "FunctionLayer/Texture/TextureFactory.h"
#include "FunctionLayer/Material/BxDF/ComplexIor.h"
std::shared_ptr < BxDF > ConductorMaterial::getBxDF(const Intersection & intersect) const  {
    Spectrum itsAlbedo = albedo->eval(intersect);
    if( roughness || uRoughness || vRoughness){
        double  uRough = uRoughness?uRoughness->eval(intersect):roughness->eval(intersect);
        double  vRough = uRoughness?uRoughness->eval(intersect):roughness->eval(intersect);
        if(glint)
            return std::make_shared <GlintBxDF>(eta,k,itsAlbedo, uRough, vRough, intersect, distrib);
        return std::make_shared <RoughConductorBxDF>(eta,k,itsAlbedo,uRough,vRough,distrib);
        //Rough conductor
    }
    else {
        return  std::make_shared<ConductorBxDF>(eta,k,itsAlbedo);
    }

}

std::shared_ptr < BSSRDF > ConductorMaterial::getBSSRDF(const Intersection & intersect) const{
    return std::shared_ptr < BSSRDF >();
}



ConductorMaterial::ConductorMaterial(const Json & json): Material(json) {
    albedo = TextureFactory::LoadTexture<RGB3>(json,"albedo",RGB3(1,1,1));
    eta = getOptional(json,"eta",Vec3d(0.2004376970, 0.9240334304, 1.1022119527));
    k   = getOptional(json,"k", Vec3d(3.9129485033f, 2.4528477015f,
                                      2.1421879552f));
    conductorName = getOptional(json,"conductor",std::string());
    conductorName = getOptional(json,"material",conductorName);
    glint = getOptional(json, "glint", false);
    if(json.contains("roughness"))
        roughness = TextureFactory::LoadTexture<double>(json["roughness"]);
    if(json.contains("u_roughness"))
        uRoughness =  TextureFactory::LoadTexture<double>(json["u_roughness"]);
    if(json.contains("v_roughness"))
        vRoughness =  TextureFactory::LoadTexture<double>(json["v_roughness"]);
    if(roughness || uRoughness || vRoughness){
        distrib = LoadDistributionFromJson(json);
    }
    if(!conductorName.empty())
        ComplexIorList::lookup(conductorName,eta,k);

}
