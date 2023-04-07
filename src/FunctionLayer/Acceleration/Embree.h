/**
 * @file Embree.h
 * @author Chenxi Zhou
 * @brief 
 * @version 0.1
 * @date 2022-09-25
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#pragma once

#include "Accel.h"
#include "embree3/rtcore.h"

/// @brief Acceleration structure built on Intel Embree. https://www.embree.org/
struct EmbreeAccel : public Accel {

    EmbreeAccel(const std::vector<std::shared_ptr<Entity>> &_entities);

    virtual std::optional<Intersection> Intersect(const Ray &r) const override;

    [[nodiscard]]
    BoundingBox3f getGlobalBoundingBox() const override {
        RTCBounds aabb{};
        rtcGetSceneBounds(scene, &aabb);
        Point3d pMin(aabb.lower_x, aabb.lower_y, aabb.lower_z);
        Point3d pMax(aabb.upper_x, aabb.upper_y, aabb.upper_z);
        return {pMin, pMax};
    }

private:
    std::vector<std::shared_ptr<Entity>> entities;
    //* Embree
    RTCDevice device;
    RTCScene scene;   
};