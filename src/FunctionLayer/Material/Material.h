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
#include <memory>
#include <utility>
#include "CoreLayer/Adapter/JsonUtil.h"
#include "FunctionLayer/Material/BxDF/BxDF.h"
template<class T>
class Texture;

struct Intersection;

class Medium;
class BSSRDF;

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

class Ray;
class Material
{
public:
    Material(const Json & json);
    Material():twoSideShading(true){}
	virtual std::shared_ptr<BxDF> getBxDF(const Intersection & intersect) const ;
	virtual std::shared_ptr<BSSRDF> getBSSRDF(const Intersection & intersect) const;
    virtual  void setFrame(Intersection & its,const Ray & ray) const;
    virtual  void flipFrame(Intersection & its,const Ray & ray) const;
    std::shared_ptr<Medium> getInsideMedium() const;
    std::shared_ptr<Medium> getOutsideMedium() const;
    void setInsideMedium(std::shared_ptr<Medium> _insideMedium);
    void setOutMedium(std::shared_ptr<Medium> _outsideMedium);
    EMaterialType type = EMaterialType::Unkown;
protected:
    bool twoSideShading;
    std::shared_ptr<Medium> insideMedium;
    std::shared_ptr<Medium> outsideMedium;
};