#include "MatteMaterial.h"
#include "FunctionLayer/Material/BxDF/Diffuse.h"

MatteMaterial::MatteMaterial(const std::shared_ptr<Texture<Spectrum>> &kd) : kd(kd) {}

std::shared_ptr<BxDF> MatteMaterial::getBxDF(Intersection intersect) const
{
    auto color = kd->eval(intersect);
    std::shared_ptr<Diffuse> bxdf = std::make_shared<Diffuse>(color);
<<<<<<< HEAD:src/Material/MatteMaterial.cpp
//    return std::make_shared<Mircofacet>(color,1.5046,1.000277,0.05);
=======
>>>>>>> 9b4df9bee41fdda8d6f8f5e2560cf49eedb41697:src/FunctionLayer/Material/MatteMaterial.cpp
    return bxdf;
}

std::shared_ptr<BSSRDF> MatteMaterial::getBSSRDF(Intersection intersect) const
{
    return nullptr;
}