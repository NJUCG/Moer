/**
 * @file Material.h
 * @author orbitchen
 * @brief Material of an object. Generate BxDF and/or BSSRDF.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "FunctionLayer/Material/BxDF/BxDF.h"
#include "FunctionLayer/Material/BSSRDF/BSSRDF.h"
#include <memory>
#include <utility>

template<class T>
class Texture;

struct Intersection;

class Medium;

class Material
{
public:
    Material(const std::shared_ptr<Texture<Spectrum>> &  _albedo = nullptr,
             const std::shared_ptr<Texture<double>> &  _bump = nullptr);
	virtual std::shared_ptr<BxDF> getBxDF(const Intersection & intersect) const ;
	virtual std::shared_ptr<BSSRDF> getBSSRDF(const Intersection & intersect) const;

    std::shared_ptr<Medium> getInsideMedium() const;
    std::shared_ptr<Medium> getOutsideMedium() const;

protected:
    std::shared_ptr<Texture<Spectrum>> albedo;
    std::shared_ptr<Texture<double>> bump;

    std::shared_ptr<Medium> insideMedium;
    std::shared_ptr<Medium> outsideMedium;
};