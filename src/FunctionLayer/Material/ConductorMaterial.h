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

class ConductorMaterial : public  Material{
public:
    ConductorMaterial(Vec3d _eta,Vec3d _k,
                      std::shared_ptr<Texture<Spectrum>> _albedo = nullptr,
                      std::shared_ptr<Texture<double>> _bump = nullptr
            );
    ConductorMaterial(const std::string  & _materialName);

    std::shared_ptr < BxDF > getBxDF(const Intersection & intersect)  const override;
//
    std::shared_ptr < BSSRDF > getBSSRDF(const Intersection & intersect) const override;
private:
    Vec3d eta;
    Vec3d k;
    std::string materialName;
};