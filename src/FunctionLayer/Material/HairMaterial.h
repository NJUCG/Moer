#include "Material.h"
#include "BxDF/HairBXDF.h"
class Sampler;
class HairMaterial : public  Material {
public:
    HairMaterial(const Json & json);

    std::shared_ptr < BxDF > getBxDF(const Intersection & intersect) const override;
    void setFrame(Intersection &its, const Ray &ray) const override;

private:
    HairAttribute attr;
    std::shared_ptr <Texture<double>> roughness;
};
//class RoughHairMaterial : public  Material {
//public:
//    RoughHairMaterial(const Json & json);
//
//    std::shared_ptr < BxDF > getBxDF(const Intersection & intersect) const override;
//private:
//    HairAttribute attr;
//    std::shared_ptr <Texture<double>> roughness;
//    std::shared_ptr<Sampler> sampler;
//};