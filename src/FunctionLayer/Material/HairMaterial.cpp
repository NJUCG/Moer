#include "HairMaterial.h"
#include "FunctionLayer/Texture/TextureFactory.h"
#include "FunctionLayer/Sampler/Independent.h"
HairMaterial::HairMaterial(const Json & json) {
    attr.scaleAngle = getOptional(json, "scale_angle", 2.5);
    attr.scaleAngle *= M_PI / 180;
    attr.overrideSigmaA = containsAndGet(json, "sigma_a", attr.sigmaA);
    attr.melaninRatio = getOptional(json, "melanin_ratio", 1);
    attr.melaninConcentration = getOptional(json, "melanin_concentration", 1.3);
    roughness = TextureFactory::LoadTexture <double>(json,"roughness",0.1);
    twoSideShading = false;
}


std::shared_ptr < BxDF > HairMaterial::getBxDF(const Intersection & intersect) const {
    return std::make_shared <Hair>(&attr,roughness->eval(intersect),2 * intersect.uv.y - 1 );
}





///For hair material,tangent is necessary.
void HairMaterial::setFrame(Intersection &its, const Ray &ray) const {
    its.shFrame = Frame(its.geometryNormal);
    return;
}
