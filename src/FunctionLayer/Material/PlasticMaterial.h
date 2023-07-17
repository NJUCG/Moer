#include "Material.h"
class MicrofacetDistribution;
class PlasticMaterial : public Material{
public:
    PlasticMaterial(const Json & json);

    std::shared_ptr < BxDF > getBxDF(const Intersection & intersect) const override;

    std::shared_ptr < BSSRDF > getBSSRDF(const Intersection & intersect) const override;

private:
    std::shared_ptr<Texture<double>> roughness = nullptr, uRoughness = nullptr, vRoughness = nullptr;
    std::shared_ptr<MicrofacetDistribution> distrib;
    std::shared_ptr<Texture<RGB3>> albedoSpecular,albedoDiffuse;
    double _diffuseFresnel;
    double _avgTransmittance;
    Spectrum _scaledSigmaA;
    double ior;
};