/**
 * @file Material.cpp
 * @author JunPing Yuan
 * @brief 
 * @version 0.1
 * @date 2022/9/6
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Material.h"
#include "CoreLayer/Ray/Ray.h"
#include "FunctionLayer/Texture/Texture.h"

static std::shared_ptr<Texture<Spectrum>> defaultSpectrum = std::make_shared<ConstantTexture<Spectrum>>(Spectrum(RGB3(1, 1, 1)));


std::shared_ptr < BxDF > Material::getBxDF(const Intersection & intersect) const  {
    return nullptr;
}


std::shared_ptr < BSSRDF > Material::getBSSRDF(const Intersection & intersect) const {
    return std::shared_ptr <BSSRDF>();
}

std::shared_ptr<Medium> Material::getInsideMedium() const {
    return insideMedium;
}

std::shared_ptr<Medium> Material::getOutsideMedium() const {
    return outsideMedium;
}

void Material::setInsideMedium(std::shared_ptr < Medium > _insideMedium) {
    insideMedium = _insideMedium;
}

void Material::setOutMedium(std::shared_ptr < Medium > _outsideMedium) {
    outsideMedium = _outsideMedium;
}
void Material::setFrame(Intersection &its,const Ray&  ray) const{
    its.shFrame = Frame(its.geometryNormal);
    if(twoSideShading)
        flipFrame(its,ray);
}

void Material::flipFrame(Intersection &its, const Ray &ray) const {
    bool hitBackSide = dot(-ray.direction, its.geometryNormal) < 0;
    if (hitBackSide) {
        its.shFrame.n = -its.shFrame.n;
        its.shFrame.s = -its.shFrame.s;
    }
}

Material::Material(const Json &json) {
    twoSideShading = getOptional(json,"two_side_shading",true);
}
