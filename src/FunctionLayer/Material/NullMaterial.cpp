#include "NullMaterial.h"
#include "FunctionLayer/Medium/Beerslaw.h"
#include "FunctionLayer/Medium/Homogeneous.h"
NullMaterial::NullMaterial(const Json &json) {
    type = EMaterialType::Null;

    insideMedium = std::make_shared<HomogeneousMedium>(10, 1);
}

std::shared_ptr<BxDF>
NullMaterial::getBxDF(const Intersection &intersect) const 
{
    return nullptr;
}

std::shared_ptr<BSSRDF>
NullMaterial::getBSSRDF(const Intersection &intersect) const 
{
    return nullptr;
}
