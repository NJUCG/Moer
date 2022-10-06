/**
 * @file Entity.h
 * @author orbitchen
 * @brief Objects that can be intersected with ray.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "CoreLayer/Geometry/Transform3d.h"
#include "FunctionLayer/Material/Material.h"
#include "CoreLayer/Geometry/BoundingBox.h"
#include "CoreLayer/Ray/Ray.h"
#include "CoreLayer/Adapter/JsonUtil.hpp"
#include <embree3/rtcore.h>
#include <optional>
#include <memory>

struct EntitySurfaceInfo {
	Point3d position;
	Normal3d normal;
	Normal3d tangent;
	Normal3d bitangent;
	Point2d uv;
};


struct Intersection;
class Light;

/**
 * @brief User defined RayHit1
 * 
 */
struct UserRayHit1 : public RTCRayHit {
	std::shared_ptr<Intersection> its = nullptr;
};

class Entity : public Transform3D
{
public:
	std::shared_ptr<Light> lightPtr;
	std::shared_ptr<Material> material;
	//@brief Returns the intersection of the entity and the ray
	virtual std::optional<Intersection> intersect(const Ray &r) const = 0;
    //@brief Return ptr to light when primitive is a emitter. Otherwise, return nullptr.
	virtual std::shared_ptr<Light> getLight() const = 0;
	virtual void setLight(std::shared_ptr<Light> light) = 0;
	virtual double area() const = 0;
	virtual Intersection sample(const Point2d &positionSample) const = 0;
	//@brief Return the bounding box of the entity
	virtual BoundingBox3f WorldBound() const = 0;
    //@brief set the material of the entity
    void setMaterial(std::shared_ptr<Material> _material){
        material = _material;
    }
    Entity(){}
    Entity(const Json json) : Transform3D(json.at("transform")){
		
    }

	virtual RTCGeometry toEmbreeGeometry(RTCDevice device) const;

	virtual std::optional<Intersection> getIntersectionFromRayHit(const UserRayHit1 &rayhit) const;

	virtual EntitySurfaceInfo
	getEntitySurfaceInfo(int instID, Point2d uv) const = 0;

};


