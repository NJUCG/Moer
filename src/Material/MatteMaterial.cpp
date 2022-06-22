
#include "MatteMaterial.h"

std::shared_ptr<BxDF> MatteMaterial::getBxDF(Intersection intersect) const
{
    auto color = kd->eval(intersect);
    std::shared_ptr<Diffuse> bxdf = std::make_shared<Diffuse>(color);
//    return std::make_shared<Mircofacet>(color,1.5046,1.000277,0.05);
    return bxdf;
}
std::shared_ptr<BSSRDF> MatteMaterial::getBSSRDF(Intersection intersect) const
{
    return nullptr;
}