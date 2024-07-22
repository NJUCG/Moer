#pragma once
#include "Material.h"
#include "FunctionLayer/Material/BxDF/DielectricBxDF.h"
#include "FunctionLayer/Intersection.h"
#include "FunctionLayer/Texture/Texture.h"

class PourousLayerMicrograinMaterial : public Material {
public:
    enum MicrograinType {
        CONDUCTOR,
        PLASTIC,
    };
    std::shared_ptr<BxDF> getBxDF(const Intersection &intersect) const override;

    std::shared_ptr<BSSRDF> getBSSRDF(const Intersection &intersect) const override;

    PourousLayerMicrograinMaterial(const Json &json);

private:
    MicrograinType micrograinType;
    std::shared_ptr<Material> bulkMaterial;
    std::shared_ptr<Texture<double>> tau0;
    std::shared_ptr<Texture<double>> beta;
    std::shared_ptr<Texture<Spectrum>> R0;
    Vec3d k;
    std::shared_ptr<Texture<Spectrum>> kd;
};
