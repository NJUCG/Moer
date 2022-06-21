/**
 * @file Sampler.h
 * @author orbitchen
 * @brief generate random numbers between (0,1).
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <vector>

#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Math/Common.h"
class Sampler
{
public:
	virtual double sample() const = 0;

	virtual std::vector<double> sample(int num) const = 0;
};
