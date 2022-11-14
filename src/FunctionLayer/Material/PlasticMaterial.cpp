#include "PlasticMaterial.h"
#include "BxDF/PlasticBxDF.h"
#include "FunctionLayer/Texture/Texture.h"
#include "FunctionLayer/Texture/TextureFactory.h"
#include "FunctionLayer/Material/BxDF/MicrofacetDistribution.h"

std::shared_ptr < BxDF > PlasticMaterial::getBxDF(const Intersection & intersect) const {
    Spectrum itsAlbedoDiffuse = albedoDiffuse->eval(intersect);
    if ( roughness || uRoughness || vRoughness ) {
        Spectrum itsAlbedoGlossy = albedoSpecular->eval(intersect);
        double uRough = uRoughness ? uRoughness->eval(intersect) : roughness->eval(intersect);
        double vRough = uRoughness ? uRoughness->eval(intersect) : roughness->eval(intersect);
        return std::make_shared < RoughPlastic >(itsAlbedoGlossy, itsAlbedoDiffuse, ior, uRough, vRough, distrib);
    } else {
        Spectrum itsAlbedoSpecular = albedoSpecular->eval(intersect);
        return std::make_shared < Plastic >(itsAlbedoSpecular, itsAlbedoDiffuse, ior);
    }
}

std::shared_ptr < BSSRDF > PlasticMaterial::getBSSRDF(const Intersection & intersect) const {
    return Material::getBSSRDF(intersect);
}

PlasticMaterial::PlasticMaterial(const Json & json) {
    ior = getOptional(json, "ior", 1.5);
    albedoSpecular = TextureFactory::LoadTexture<>(json,"albedo_specular", RGB3(1, 1, 1));
    albedoDiffuse = TextureFactory::LoadTexture<>(json,"albedo_diffuse", RGB3(0.5, 0.5, 0.5));
    if ( json.contains("roughness") )
        roughness = TextureFactory::LoadTexture < double >(json["roughness"]);
    if ( json.contains("u_roughness") )
        uRoughness = TextureFactory::LoadTexture < double >(json["u_roughness"]);
    if ( json.contains("v_roughness") )
        vRoughness = TextureFactory::LoadTexture < double >(json["v_roughness"]);
    if ( roughness || uRoughness || vRoughness ) {
        distrib = LoadDistributionFromJson(json);
    }
}


