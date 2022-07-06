/**
 * @file
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
#include "FunctionLayer/Texture/Texture.h"

class MatteMaterial     : public  Material{
private:
    std::shared_ptr<Texture<Spectrum>> kd ;
public:
    MatteMaterial(const std::shared_ptr<Texture<Spectrum>> & kd );

    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;

    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;

};


