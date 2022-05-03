/**
 * @file Texture.h
 * @author orbitchen
 * @brief Texture of different types.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Intersection.h"

// Tvalue can be float or any specturm type
template <typename Tvalue>
class Texture
{
public:
	virtual Tvalue eval(const Intersection &intersection) const = 0;
};
