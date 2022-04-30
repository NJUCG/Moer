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

#include "BxDF.h"
#include "BSSRDF.h"
#include "Intersection.h"

class Material
{
public:
	
	std::shared_ptr<BxDF> getBxDF(Intersection intersect) const=0;

	std::shared_ptr<BSSRDF> getBSSRDF(Intersection intersect) const=0;

};