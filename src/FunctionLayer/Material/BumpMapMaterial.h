#pragma once
#include "Material.h"

class BumpMaterial : public Material{
public:
    BumpMaterial(const Json & json);
    std::shared_ptr<BxDF> getBxDF(const Intersection &intersect) const override;
    std::shared_ptr<BSSRDF> getBSSRDF(const Intersection &intersect) const override;
    void setFrame(Intersection &its,const Ray & ray) const override;
    std::shared_ptr<Texture<RGB3>> bump = nullptr;//todo: replace this with scale texture
    std::shared_ptr<Material> innerMaterial;
};
