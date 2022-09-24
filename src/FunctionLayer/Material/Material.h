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
#include "CoreLayer/Adapter/JsonUtil.hpp"

template<class T>
class Texture;

struct Intersection;

class Medium;

enum EMaterialType {
    Unkown = 1 << 0,
    Null = 1 << 1,
    Diffuse = 1 << 2,
    SpecularReflect = 1 << 3, 
    RoughReflect = 1 << 4,
    SpecularTransmission = 1 << 5,
    RoughTransmission = 1 << 6
    
    //* Other
};


class Material
{
public:
    Material(const std::shared_ptr<Texture<Spectrum>> &  _albedo = nullptr,
             const std::shared_ptr<Texture<double>> &  _bump = nullptr);
	virtual std::shared_ptr<BxDF> getBxDF(const Intersection & intersect) const ;
	virtual std::shared_ptr<BSSRDF> getBSSRDF(const Intersection & intersect) const;

    std::shared_ptr<Medium> getInsideMedium() const;
    std::shared_ptr<Medium> getOutsideMedium() const;
    void setInsideMedium(std::shared_ptr<Medium> _insideMedium);
    void setOutMedium(std::shared_ptr<Medium> _outsideMedium);


    EMaterialType type = EMaterialType::Unkown;

    EMaterialType type = EMaterialType::Unkown;

protected:
    std::shared_ptr<Texture<Spectrum>> albedo;
    std::shared_ptr<Texture<double>> bump;

    std::shared_ptr<Medium> insideMedium;
    std::shared_ptr<Medium> outsideMedium;
};