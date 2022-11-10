/**
 * @file Sphere.h
 * @author Zhimin Fan
 * @brief Sphere. Only for test. Ignore transform.
 * @version 0.1
 * @date 2022-05-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <optional>
#include "Entity.h"

class Sphere : public Entity
{
public:
    Sphere(const Json & json);

    Sphere(Point3d _center, double _radius, std::shared_ptr<Material> _material);

    virtual std::optional<Intersection> intersect(const Ray &r) const override;

    virtual double area() const override;

    virtual Intersection sample(const Point2d &positionSample) const override;

	virtual BoundingBox3f WorldBound() const override;

protected:
    double radius;
    Point3d center;
    virtual void apply() override;


};