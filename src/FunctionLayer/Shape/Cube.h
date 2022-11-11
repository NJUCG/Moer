/**
 * @file Cube.h
 * @author JunPing Yuan
 * @brief Cube Shape
 * @version 0.1 to implement..
 * @date 2022-09-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <optional>
#include <memory>
#include "Entity.h"
#include "Triangle.h"
#include "ResourceLayer/File/MeshData.h"
#include "FunctionLayer/Acceleration/Bvh.h"

class Cube : public  Entity{
public:
    Cube() = default;
    
    Cube(const Json &json);

    virtual std::optional<Intersection> intersect(const Ray &r) const override;

    virtual double area() const override;

    virtual Intersection sample(const Point2d &positionSample) const override;

    virtual BoundingBox3f WorldBound() const override;

protected:
    Point3d position;
    Matrix4x4 rotation;
    Matrix4x4 scale;

    virtual void apply() override;
};
