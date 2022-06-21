#include "MatteMaterial.h"
#include "FunctionLayer/Material/BxDF/Diffuse.h"

MatteMaterial::MatteMaterial(const std::shared_ptr<Texture<Spectrum>> &kd) : kd(kd) {}

std::shared_ptr<BxDF> MatteMaterial::getBxDF(Intersection intersect) const
{
    auto color = kd->eval(intersect);
    std::shared_ptr<Diffuse> bxdf = std::make_shared<Diffuse>(color);
    return bxdf;
}

std::shared_ptr<BSSRDF> MatteMaterial::getBSSRDF(Intersection intersect) const
{
    return nullptr;
}