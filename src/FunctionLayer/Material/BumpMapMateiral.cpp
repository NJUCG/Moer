/**
 * @file 
 * @author JunPing Yuan
 * @brief 
 * @version 0.1
 * @date 2023/3/26
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "BumpMapMaterial.h"
#include "FunctionLayer/Intersection.h"
#include "FunctionLayer/Texture/TextureFactory.h"
#include "FunctionLayer/Material/MaterialFactory.h"
std::shared_ptr<BxDF> BumpMaterial::getBxDF(const Intersection &intersect) const {
    return innerMaterial->getBxDF(intersect);
}

std::shared_ptr<BSSRDF> BumpMaterial::getBSSRDF(const Intersection &intersect) const {
    return innerMaterial->getBSSRDF(intersect);
}

void BumpMaterial::setFrame(Intersection &its, const Ray &ray) const {
    if (!bump) {
        its.shFrame = Frame(its.geometryNormal);
    }
    if (bump) {
        Frame frame = Frame(its.geometryNormal);
        Intersection evalIts(its);
        //todo: compute du and dv in intersect.
        auto du = 0.0005;
        auto dv = 0.0005;
        evalIts.uv.x += du;
        double uDisplace = RGBSpectrum(bump->eval(evalIts)).luminance();
        evalIts.uv = its.uv + Vec2d(0.0, dv);
        double vDisplace = RGBSpectrum(bump->eval(evalIts)).luminance();
        double displace = RGBSpectrum(bump->eval(its)).luminance();
        auto t = frame.s + (uDisplace - displace) * 0.1 / du * frame.n;
        auto b = frame.t + (vDisplace - displace) * 0.1 / dv * frame.n;
        auto n = normalize(cross(t, b));
        t = normalize(t);
        its.shFrame = Frame(t, cross(n, t), n);
        return;
    }
}

BumpMaterial::BumpMaterial(const Json &json) {
    if (json.contains("bump"))
        bump = TextureFactory::LoadTexture<RGB3>(json.at("bump"));
    innerMaterial = MaterialFactory::LoadMaterialFromJson(json.at("material"));
}
