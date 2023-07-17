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

/// @brief Acceleration structure Interface.
class Accel {

public:

    Accel() = default;

    /*
    * @brief basic interface for ray intersection.
    * @param r The ray that intersect with a scene. This interface will only utilize its geometry information.
    * @return The exactly closest intersection point on an entity in the scene with other useful information.
    */
    virtual std::optional<Intersection> Intersect(const Ray &r) const = 0;

    /*
    * @brief get the bounding box of all the objects
    */
    [[nodiscard]]
    virtual BoundingBox3f getGlobalBoundingBox() const = 0;
};