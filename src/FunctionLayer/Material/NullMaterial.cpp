#include "NullMaterial.h"
#include "FunctionLayer/Medium/Beerslaw.h"
NullMaterial::NullMaterial(const Json &json) {
    type = EMaterialType::Null;

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
