/**
 * @file MatteMaterial.h
 * @author Junping Yuan
 * @brief
 * @version 0.1
 * @date 2022/6/7
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma  once
#include "Material.h"
#include "FunctionLayer/Intersection.h"

class MatteMaterial   : public  Material{

public:
    MatteMaterial(const std::shared_ptr<Texture<Spectrum>> & _albedo = nullptr,
                  const std::shared_ptr<Texture<double>> & _bump = nullptr );
    MatteMaterial(const nlohmann::json &);
    std::shared_ptr<BxDF> getBxDF(const Intersection & intersect) const   override;

    std::shared_ptr<BSSRDF> getBSSRDF(const Intersection & intersect) const  override;
protected:

};


