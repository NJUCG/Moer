#include "PlasticMaterial.h"
#include "BxDF/PlasticBxDF.h"
#include "FunctionLayer/Texture/Texture.h"
#include "FunctionLayer/Texture/TextureFactory.h"
#include "FunctionLayer/Material/BxDF/MicrofacetDistribution.h"
#include "FunctionLayer/Material/BxDF/Fresnel.h"
std::shared_ptr < BxDF > PlasticMaterial::getBxDF(const Intersection & intersect) const {
    Spectrum itsAlbedoDiffuse = albedoDiffuse->eval(intersect);
    if ( roughness || uRoughness || vRoughness ) {
        double uRough = uRoughness ? uRoughness->eval(intersect) : roughness->eval(intersect);
        double vRough = uRoughness ? uRoughness->eval(intersect) : roughness->eval(intersect);
        return std::make_shared < RoughPlastic >(itsAlbedoDiffuse,ior,_diffuseFresnel,_avgTransmittance,_scaledSigmaA, uRough, vRough, distrib);
    } else {
        return std::make_shared<Plastic>(itsAlbedoDiffuse,ior,_diffuseFresnel,_avgTransmittance,_scaledSigmaA);
    }
}

std::shared_ptr < BSSRDF > PlasticMaterial::getBSSRDF(const Intersection & intersect) const {
    return Material::getBSSRDF(intersect);
}

PlasticMaterial::PlasticMaterial(const Json & json): Material(json) {
    ior = getOptional(json, "ior", 1.5);
    albedoDiffuse = TextureFactory::LoadTexture<>(json,"albedo", RGB3(0.5, 0.5, 0.5));
    if ( json.contains("roughness") )
        roughness = TextureFactory::LoadTexture < double >(json["roughness"]);
    if ( json.contains("u_roughness") )
        uRoughness = TextureFactory::LoadTexture < double >(json["u_roughness"]);
    if ( json.contains("v_roughness") )
        vRoughness = TextureFactory::LoadTexture < double >(json["v_roughness"]);
    if ( roughness || uRoughness || vRoughness ) {
        distrib = LoadDistributionFromJson(json);
    }
    double thickness = getOptional(json, "thickness", 1);
    Spectrum sigmaA = getOptional(json, "sigma_a", Spectrum(0.f));
    _scaledSigmaA = thickness * sigmaA;
    _avgTransmittance = std::exp(-2.0 * _scaledSigmaA.average());
    _diffuseFresnel = Fresnel::diffuseReflectance(ior, 10000);
}


