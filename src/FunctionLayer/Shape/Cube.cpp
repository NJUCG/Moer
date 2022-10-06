#include "Cube.h"

Cube::Cube(const Json &json) : Entity(json) {
    position = matrix->getTranslate() * Point3d(.0f);
    scale = matrix->getScale();
    rotation = matrix->getRotate();
}

// todo
std::optional<Intersection> Cube::intersect(const Ray &r) const {
    Matrix4x4 invRotation = rotation.inverse();

    //* ori_ is the r.origin in cube coordinate system
    //* dir_ is the r.direction in cube coordinate system 
    //* That is, rotate the ray other than the cube
    auto ori_ = invRotation * (r.origin - position);
    auto dir_ = invRotation * r.direction;

    //* Now, the cube is an axis-aligned box
    auto pMin = - (scale * Point3d(1)),
         pMax = scale * Point3d(1);

    auto tmin = r.timeMin,
         tmax = r.timeMax;


    //todo divide 0?
    for (int i = 0; i < 3; ++i) {
        auto invD = 1 / dir_[i];
        if (invD >= 0) {
            tmin = std::max(tmin, (pMin[i] - ori_[i]) * invD);
            tmax = std::min(tmax, (pMax[i] - ori_[i]) * invD);
        } else {
            tmax = std::min(tmax, (pMin[i] - ori_[i]) * invD);
            tmin = std::max(tmin, (pMax[i] - ori_[i]) * invD);
        }
    }

    if (tmin > tmax)
        return std::nullopt;

    auto computeNormalInLocal = [&pMin, &pMax](Vec3d hitpoint_)
    {
        int minDimension = -1;
        double minBias = 1e10;
        for (int i = 0; i < 3; ++i) {
            if (std::abs(pMin[i] - hitpoint_[i]) < minBias) {
                minDimension = 2 * i;
                minBias = std::abs(std::abs(pMin[i] - hitpoint_[i]));
            }
            if (std::abs(pMax[i] - hitpoint_[i]) < minBias) {
                minDimension = 2 * i + 1;
                minBias = std::abs(pMax[i] - hitpoint_[i]);
            }
        }
        assert(minDimension != -1);
        Vec3d normal{0};
        normal[minDimension/2] = (minDimension%2) ? 1 : -1;
        return normal;
    };

    Intersection its;
    if (tmin > r.timeMin && tmin < r.timeMax) {
        its.t = tmin;
        Vec3d hitpoint_ = ori_ + its.t * dir_;
        its.geometryNormal = rotation * computeNormalInLocal(hitpoint_);
        its.position = *matrix * (Point3d(0) + hitpoint_);
        its.object = this;
        its.shFrame = Frame{its.geometryNormal};
        its.material = material;
        return std::make_optional(its);
    } else if (tmax > r.timeMin && tmax < r.timeMax) {
        its.t = tmax;
        Vec3d hitpoint_ = ori_ + its.t * dir_;
        its.geometryNormal = rotation * computeNormalInLocal(hitpoint_);
        its.position = *matrix * (Point3d(0) + hitpoint_);
        its.object = this;
        its.shFrame = Frame{its.geometryNormal};
        its.material = material;
        return std::make_optional(its);
    }
    return std::nullopt;
}

std::shared_ptr<Light> Cube::getLight() const {
    return this->lightPtr;
}

void Cube::setLight(std::shared_ptr<Light> light) {
    this->lightPtr = light;
}

double Cube::area() const {
    Vec3d edge = scale * Vec3d(1);
    return 2 * (edge.x * edge.y + edge.x * edge.z + edge.y * edge.z);
}

// todo
Intersection Cube::sample(const Point2d &positionSample) const {
    return Intersection();
}

BoundingBox3f Cube::WorldBound() const {
    BoundingBox3f box;
    Vec3d edge = scale * Vec3d(1);

    for (int i = 0; i < 8; ++i) {
        BoundingBoxPointUnion(box, 
            position +  rotation * Vec3d(
                (i & 1 ? -edge.x : edge.x),
                (i & 2 ? -edge.y : edge.y),
                (i & 4 ? -edge.z : edge.z)
            )
        );
    }
    return box;
}

void rtcCubeBoundsFunc(const RTCBoundsFunctionArguments *args)
{
    Cube *cube = (Cube *)args->geometryUserPtr;
    RTCBounds *bounds = args->bounds_o;

    auto [pMin, pMax] = cube->WorldBound();

    bounds->lower_x = pMin.x;
    bounds->lower_y = pMin.y;
    bounds->lower_z = pMin.z;

    bounds->upper_x = pMax.x;
    bounds->upper_y = pMax.y;
    bounds->upper_z = pMax.z;
}

void rtcCubeIntersectFunc(const RTCIntersectFunctionNArguments *args)
{
    Cube *cube = (Cube *)args->geometryUserPtr;

    RTCRayHitN *rayhit = args->rayhit;
    RTCRayN *ray = RTCRayHitN_RayN(rayhit, 1);
    RTCHitN *hit = RTCRayHitN_HitN(rayhit, 1);

    auto ori_x = RTCRayN_org_x(ray, 1, 0),
         ori_y = RTCRayN_org_y(ray, 1, 0),
         ori_z = RTCRayN_org_z(ray, 1, 0),
         dir_x = RTCRayN_dir_x(ray, 1, 0),
         dir_y = RTCRayN_dir_y(ray, 1, 0),
         dir_z = RTCRayN_dir_z(ray, 1, 0);

    auto tnear = RTCRayN_tnear(ray, 1, 0),
         tfar = RTCRayN_tfar(ray, 1, 0);

    Point3d ori{ori_x, ori_y, ori_z};
    Vec3d dir{dir_x, dir_y, dir_z};

    Ray r{ori, dir, tnear, tfar};
    auto its = cube->intersect(r);

    if (!its.has_value())
        return;

    RTCHit result;
    result.geomID = args->geomID;
    result.u = its->uv.x;
    result.v = its->uv.y;


    *((RTCHit *)hit) = result;
    RTCRayN_tfar(ray, 1, 0) = its->t;
}

/*
RTCGeometry Cube::toEmbreeGeometry(RTCDevice device) const {
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryUserPrimitiveCount(geom, 1);
    rtcSetGeometryUserData(geom, (void *)this);
    rtcSetGeometryBoundsFunction(geom, rtcCubeBoundsFunc, nullptr);
    rtcSetGeometryIntersectFunction(geom, )
}
*/

// todo
EntitySurfaceInfo Cube::getEntitySurfaceInfo(int instID, Point2d uv) const
{
    return EntitySurfaceInfo();
}

// todo
void Cube::apply() {
    return;
}