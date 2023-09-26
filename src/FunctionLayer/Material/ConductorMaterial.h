/**
 * @file ConductorMaterial.h
 * @author JunPing Yuan
 * @brief Conductor Material
 * @version 0.1
 * @date 2022/9/6
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#pragma once

#include "Material.h"

class MicrofacetDistribution;

/* The refractive index of conductor materials is a complex number, which is specified in fresnel.h. */


class ConductorMaterial : public  Material{
public:
    ConductorMaterial(const Json & json);
    ConductorMaterial(const std::string  & _materialName);

    std::shared_ptr < BxDF > getBxDF(const Intersection & intersect)  const override;

    std::shared_ptr < BSSRDF > getBSSRDF(const Intersection & intersect) const override;
private:
    std::shared_ptr<Texture<double>> roughness = nullptr, uRoughness = nullptr, vRoughness = nullptr;
    std::shared_ptr<MicrofacetDistribution> distrib;
    Vec3d eta;
    Vec3d k;
    std::string conductorName;
    std::shared_ptr<Texture<RGB3>> albedo;
    bool glint;
};