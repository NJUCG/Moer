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

#include <optional>
#include <memory>
#include <embree3/rtcore.h>
#include "CoreLayer/Geometry/Transform3d.h"
#include "FunctionLayer/Material/Material.h"
#include "CoreLayer/Geometry/BoundingBox.h"
#include "CoreLayer/Ray/Ray.h"
#include "CoreLayer/Adapter/JsonUtil.h"
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
    Entity() = default;
	
    Entity(const Json & json);
	
	std::shared_ptr<Light> getLight() const;

	void setLight(std::shared_ptr<Light> _light);

	std::shared_ptr<Material> getMaterial() const;

    void setMaterial(std::shared_ptr<Material> _material);
	
	//* If entity is a user-defined shape, not override this function
	virtual RTCGeometry toEmbreeGeometry(RTCDevice device) const;

	//* If entity is a user-defined shape, not override this function	
	virtual std::optional<Intersection> getIntersectionFromRayHit(const UserRayHit1 &rayhit) const;

	virtual std::optional<Intersection> intersect(const Ray &r) const = 0;

	virtual double area() const = 0;

	virtual Intersection sample(const Point2d &positionSample) const = 0;
	
	virtual BoundingBox3f WorldBound() const = 0;

protected:

	std::shared_ptr<Light> light;

	std::shared_ptr<Material> material;
};


