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

#include "Entity.h"
#include <optional>

class Sphere : public Entity
{
protected:
    double radius;
    Point3d center;
    virtual void apply() override;

public:
    Sphere(const Json & json);
    Sphere(Point3d _center, double _radius, std::shared_ptr<Material> _material);
    virtual std::optional<Intersection> intersect(const Ray &r) const;
    virtual double area() const;
    virtual Intersection sample(const Point2d &positionSample) const;
    virtual std::shared_ptr<Light> getLight() const;
    virtual void setLight(std::shared_ptr<Light> light);
	virtual BoundingBox3f WorldBound() const;

    //TODO
    virtual RTCGeometry toEmbreeGeometry(RTCDevice device) const;
    //TODO
	virtual EntitySurfaceInfo
	getEntitySurfaceInfo(int instID, Point2d uv) const;

    friend void rtcSphereIntersectFunc(const RTCIntersectFunctionNArguments *args);
};