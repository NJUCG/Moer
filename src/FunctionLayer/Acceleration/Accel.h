/**
 * @file Accel.h
 * @author Chenxi Zhou
 * @brief Base Acceleration structure
 * @version 0.1
 * @date 2022-09-25
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#pragma once

#include "FunctionLayer/Shape/Entity.h"
#include "FunctionLayer/Intersection.h"

struct Accel {
public:
    Accel() = default;

    virtual std::optional<Intersection> Intersect(const Ray &r) const = 0;
};