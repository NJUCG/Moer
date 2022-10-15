/**
 * @file Embree.cpp
 * @author Chenxi Zhou
 * @brief Acceleration structure using embree
 * @version 0.1
 * @date 2022-09-26
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#include "Embree.h"

RTCRay toRTCRay(const Ray &ray) {
    RTCRay rtcRay;
    rtcRay.org_x = ray.origin.x;
    rtcRay.org_y = ray.origin.y;
    rtcRay.org_z = ray.origin.z;

    rtcRay.dir_x = ray.direction.x;
    rtcRay.dir_y = ray.direction.y;
    rtcRay.dir_z = ray.direction.z;

    rtcRay.tnear = ray.timeMin;
    rtcRay.tfar = ray.timeMax;

    rtcRay.mask = -1;
    rtcRay.flags = 0;

    return rtcRay;
}

EmbreeAccel::EmbreeAccel(const std::vector<std::shared_ptr<Entity>> &_entities)
    : entities(_entities)
{     
    device = rtcNewDevice(nullptr);
    scene = rtcNewScene(device);

    for (int i = 0; i < entities.size(); ++i) {
        auto embreeGeom = entities[i]->toEmbreeGeometry(device);
        rtcAttachGeometryByID(scene, embreeGeom, i);
        rtcReleaseGeometry(embreeGeom);
    }
    rtcCommitScene(scene);
}

std::optional<Intersection> EmbreeAccel::Intersect(const Ray &r) const 
{
    RTCIntersectContext ictx;
    rtcInitIntersectContext(&ictx);

//    RTCRayHit rayhit;
    UserRayHit1 rayhit;
    rayhit.ray = toRTCRay(r);
    rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect1(scene, &ictx, &rayhit);

    int geomID = rayhit.hit.geomID,
        primID = rayhit.hit.primID;
    if (geomID == RTC_INVALID_GEOMETRY_ID) {
        return std::nullopt;
    }

    auto object = entities[geomID].get();

    return object->getIntersectionFromRayHit(rayhit);
}