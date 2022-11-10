/**
 * @file Quad.h
 * @author JunPing Yuan
 * @brief Quad Shape
 * @version 0.1
 * @date 2022-09-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <optional>
#include "Entity.h"

class Quad : public  Entity{
public:
    Quad() = default;
    
    Quad(const Json & json);
    
    virtual std::optional<Intersection> intersect(const Ray &r) const override;
    
    virtual double area() const override;

    virtual Intersection sample(const Point2d &positionSample) const override;

    virtual BoundingBox3f WorldBound() const override;

protected:
    Point3d  _base;
    Vec3d _edge0,_edge1;
    double  _area,_invArea;
    Vec2d  _invSq;

    virtual void apply() override;
};