/**
 * @file GridMedium.h
 * @author Chenxi Zhou
 * @brief Axis-aligned bounding cube for heterogeneous medium
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once
#include "Entity.h"
#include "../Intersection.h"
#include "Cube.h"
class HeterogeneousMedium;
class GridMedium : public Cube {
public:
    GridMedium() = default;

    GridMedium(const Json &json);

private:
    std::shared_ptr<HeterogeneousMedium> medium;
};