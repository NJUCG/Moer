#include "NormalMapMaterial.h"
#include "FunctionLayer/Intersection.h"
#include "FunctionLayer/Texture/TextureFactory.h"
#include "FunctionLayer/Material/MaterialFactory.h"
std::shared_ptr<BxDF> NormalMapMaterial::getBxDF(const Intersection &intersect) const {
    return innerMaterial->getBxDF(intersect);
}
std::shared_ptr<BSSRDF> NormalMapMaterial::getBSSRDF(const Intersection &intersect) const {
    return innerMaterial->getBSSRDF(intersect);
}

/* The dpdu vectors of some entities have not been computed.*/
void NormalMapMaterial::setFrame(Intersection &its, const Ray &ray) const {
    if (!normal)
        return;
    RGB3 nrgb = 2.0 * normal->eval(its) - 1.0;
    Normal3d n = normalize(Normal3d(nrgb[0], nrgb[1], nrgb[2]));
    Vec3d s = normalize(its.dpdu - dot(its.dpdu, n) * n);
    Vec3d t = normalize(cross(n, t));
    its.shFrame = Frame(s, t, n);
}

NormalMapMaterial::NormalMapMaterial(const Json &json) {
    if (json.contains("normal"))
        normal = TextureFactory::LoadTexture<RGB3>(json.at("normal"));
    innerMaterial = MaterialFactory::LoadMaterialFromJson(json.at("material"));
}
