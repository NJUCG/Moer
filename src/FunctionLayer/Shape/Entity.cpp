/**
 * @file 
 * @author JunPing Yuan
 * @brief 
 * @version 0.1
 * @date 2022/9/10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Entity.h"
#include "FunctionLayer/Intersection.h"
std::shared_ptr<Light> Entity::getLight() const {
    return light;
}

void Entity::setLight(std::shared_ptr<Light> _light) {
    this->light = _light;
}

std::shared_ptr<Material> Entity::getMaterial() const {
    return material;
}

void Entity::setMaterial(std::shared_ptr<Material> _material) {
    this->material = _material;
}

void rtcEntityBoundsFunc(const RTCBoundsFunctionArguments *args) {
    Entity *entity = static_cast<Entity *>(args->geometryUserPtr);
    RTCBounds *bounds = args->bounds_o;

    auto [pMin, pMax] = entity->WorldBound();

    bounds->lower_x = pMin.x;
    bounds->lower_y = pMin.y;
    bounds->lower_z = pMin.z;

    bounds->upper_x = pMax.x;
    bounds->upper_y = pMax.y;
    bounds->upper_z = pMax.z;
}

void rtcEntityIntersectFunc(const RTCIntersectFunctionNArguments *args) {
    int *valid = args->valid;
    if (!valid[0])
        return;

    Entity *entity = static_cast<Entity *>(args->geometryUserPtr);
    UserRayHit1 *rayhit = (UserRayHit1 *)(args->rayhit);

    auto ray = rayhit->ray;

    Ray r{
        Point3d(ray.org_x, ray.org_y, ray.org_z),
        Vec3d(ray.dir_x, ray.dir_y, ray.dir_z),
        ray.tnear,
        ray.tfar};

    auto its = entity->intersect(r);
    if (!its.has_value()) {
        return;
    } else {
        rayhit->its = std::make_shared<Intersection>(its.value());
        rayhit->ray.tfar = its->t;
        rayhit->hit.geomID = args->geomID;
    }
}

//todo finish it
void rtcEntityOccludeFunc(const RTCOccludedFunctionNArguments *args) {
    return;
}

/**
 * @brief Default define an user-type embree geometry
 * 
 * @param device 
 * @return RTCGeometry 
 */
RTCGeometry Entity::toEmbreeGeometry(RTCDevice device) const {
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryUserPrimitiveCount(geom, 1);
    rtcSetGeometryUserData(geom, (void *)this);
    rtcSetGeometryBoundsFunction(geom, rtcEntityBoundsFunc, nullptr);
    rtcSetGeometryIntersectFunction(geom, rtcEntityIntersectFunc);
    rtcSetGeometryOccludedFunction(geom, rtcEntityOccludeFunc);
    rtcCommitGeometry(geom);
    return geom;
}

//* This function is for user-defined embree geometry
std::optional<Intersection> Entity::getIntersectionFromRayHit(const UserRayHit1 &rayhit) const {
    auto itsPtr = rayhit.its;
    assert(itsPtr != nullptr);

    return std::make_optional(*itsPtr);
}

Entity::Entity(const Json &json) : Transform3D(getOptional(json, "transform", Json())) {
}
