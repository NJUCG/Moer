#include "ConductorMaterial.h"
#include "FunctionLayer/Texture/Texture.h"
#include "BxDF/ConductorBxDF.hpp"
ConductorMaterial::ConductorMaterial(Vec3d _eta, Vec3d _k, std::shared_ptr < Texture < Spectrum>> _albedo,
                                     std::shared_ptr < Texture < double>> _bump) : Material(_albedo,_bump),
                                     eta(_eta),k(_k){

}

std::shared_ptr < BxDF > ConductorMaterial::getBxDF(const Intersection & intersect) const  {
    Material::getBxDF(intersect);
    Spectrum itsAlbedo = albedo->eval(intersect);
    return std::make_shared<ConductorBxDF>(eta,k,itsAlbedo);
}

std::shared_ptr < BSSRDF > ConductorMaterial::getBSSRDF(const Intersection & intersect) const{
    return std::shared_ptr < BSSRDF >();
}


ConductorMaterial::ConductorMaterial(const std::string & _materialName) {  //todo

}
