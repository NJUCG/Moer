#pragma once
#include "Material.h"
class NormalMapMaterial : public Material{
public:
    NormalMapMaterial(const Json & json);
    std::shared_ptr<BxDF> getBxDF(const Intersection &intersect) const override;
    std::shared_ptr<BSSRDF> getBSSRDF(const Intersection &intersect) const override;
    void setFrame(Intersection &its,const Ray & ray) const override;
    std::shared_ptr<Texture<RGB3>> normal = nullptr;//todo: replace this with scale texture
    std::shared_ptr<Material> innerMaterial;
};
