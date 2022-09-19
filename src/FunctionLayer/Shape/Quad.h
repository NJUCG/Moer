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

#include "Entity.h"
#include <optional>

class Quad : public  Entity{
protected:
    Point3d  _base;
    Vec3d _edge0,_edge1;
    double  _area,_invArea;
    Vec2d  _invSq;

    virtual void apply() override;
public:
    Quad(const Json & json);
    Quad() = default;
    virtual std::optional<Intersection> intersect(const Ray &r) const;
    virtual double area() const;
    virtual Intersection sample(const Point2d &positionSample) const;
    virtual std::shared_ptr<Light> getLight() const;
    virtual void setLight(std::shared_ptr<Light> light);
    virtual BoundingBox3f WorldBound() const;

};