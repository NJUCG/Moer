/**
 * @file Material.h
 * @author orbitchen
 * @brief Material of an object. Generate BxDF and/or BSSRDF.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "FunctionLayer/Material/BxDF/BxDF.h"
#include "FunctionLayer/Material/BSSRDF/BSSRDF.h"
#include <memory>
//#include "Intersection.h"



struct Intersection;

class Material
{
public:

	virtual std::shared_ptr<BxDF> getBxDF(Intersection intersect) const = 0;
	virtual std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const = 0;
};