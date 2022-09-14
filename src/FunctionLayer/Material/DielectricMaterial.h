/**
 * @file DielectricMaterial.h
 * @author JunPing Yuan
 * @brief
 * @version 0.1
 * @date 2022/6/11
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma  once
#include "Material.h"
#include "FunctionLayer/Material/BxDF/DielectricBxDF.h"
#include "FunctionLayer/Intersection.h"
#include "FunctionLayer/Texture/Texture.h"

class DielectricMaterial : public  Material{

public:
    std::shared_ptr<BxDF> getBxDF(const Intersection & intersect) const override;

    std::shared_ptr<BSSRDF> getBSSRDF(const Intersection & intersect) const override;

    DielectricMaterial(const std::shared_ptr<Texture<double>> & _ior,
                       const std::shared_ptr<Texture<Spectrum>> &  _albedo = nullptr,
                       const std::shared_ptr<Texture<double>> &  _bump = nullptr ) : Material(_albedo,_bump),ior(_ior) {}


private:
    std::shared_ptr<Texture<double>> ior;

};


