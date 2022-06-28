/**
 * @file Sphere.h
 * @author Zhimin Fan
 * @brief Sphere. Only for test. Ignore transform.
 * @version 0.1
 * @date 2022-05-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Entity.h"
#include <optional>

class Sphere : public Entity
{
protected:
    double radius;
    Point3d center;
    virtual void apply() override;

public:
    Sphere(Point3d _center, double _radius, std::shared_ptr<Material> _material);
    virtual std::optional<Intersection> intersect(const Ray &r) const;
    virtual double area() const;
    virtual Intersection sample(const Point2d &positionSample) const;
    virtual std::shared_ptr<Light> getLight() const;
    virtual void setLight(std::shared_ptr<Light> light);
};