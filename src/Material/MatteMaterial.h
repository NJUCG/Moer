/**
 * @file
 * @author Junping Yuan
 * @brief
 * @version 0.1
 * @date 2022/6/7
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../Core/FunctionLayer/Material.h"
#include "../BxDF/Diffuse.h"
#include "../BxDF/Microfacet.h"
#include "Core/FunctionLayer/Intersection.h"
#include "Core/FunctionLayer/Texture.h"
#pragma  once
class MatteMaterial     : public  Material{
private:
    std::shared_ptr<Texture<Spectrum>> kd ;
public:
    MatteMaterial(const std::shared_ptr<Texture<Spectrum>> & kd )  : kd(kd){}

    virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const;

    virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const;

};


