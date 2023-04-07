/**
 * @file NullMaterial.h
 * @author Chenxi Zhou
 * @brief 
 * @version 0.1
 * @date 2022-09-24
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#pragma once

#include "Material.h"
#include "FunctionLayer/Intersection.h"

class NullMaterial : public Material{
public:
    NullMaterial() = default;
    NullMaterial(const Json &json);
	virtual std::shared_ptr<BxDF> getBxDF(const Intersection & intersect) const override;
	virtual std::shared_ptr<BSSRDF> getBSSRDF(const Intersection & intersect) const override;

private:
    std::shared_ptr<NullBxDF> bxdf = std::make_shared<NullBxDF>();

};