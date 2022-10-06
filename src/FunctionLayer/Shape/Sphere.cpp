/**
 * @file Sphere.cpp
 * @author Zhimin Fan
 * @brief Sphere. Only for test. Ignore transform.
 * @version 0.1
 * @date 2022-05-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "Sphere.h"
#include "FunctionLayer/Intersection.h"
#include "CoreLayer/Geometry/CoordConvertor.h"

Sphere::Sphere(Point3d _center, double _radius, std::shared_ptr<Material> _material)
    : center(_center), radius(_radius)
{
	this->material = _material;
}

void Sphere::apply()
{
}

std::optional<Intersection> Sphere::intersect(const Ray &r) const
{
    auto R = radius;
    auto C = center;
    auto o = r.origin;
    auto d = r.direction;
    auto u = o - C;
    double a = d.length2();
    double b = 2 * dot(u, d);
    double c = u.length2() - R * R;
    double delta = b * b - 4 * a * c;
    if (delta < 0)
        return std::nullopt;
    double sqrtDelta = sqrt(delta);
    double t1 = (-b + sqrtDelta) / (2.0 * a);
    double t2 = (-b - sqrtDelta) / (2.0 * a);
    bool flag = false;
    Intersection ans;
    double t = INFINITY;
    if (t1 >= 0 && t1 < t && t1 >= r.timeMin && t1 <= r.timeMax)
    {
        t = t1;
        Vec3d n = o + d * t1 - C;
        n = normalize(n);
        ans.position = o + d * t1;
        ans.geometryNormal = n;
        ans.geometryTangent = normalize(Vec3d(n.z, 0, -n.x));
        ans.geometryBitangent = normalize(cross(n, ans.geometryTangent));
        ans.material = material;
        ans.shFrame = Frame(n);
        ans.uv.x = (atan2(n.y, n.x) + M_PI) / M_PI / 2;
        ans.uv.y = acos(n.y) / M_PI;
        ans.object = this;
        flag = true;
    }
    if (t2 >= 0 && t2 < t && t2 >= r.timeMin && t2 <= r.timeMax)
    {
        t = t2;
        Vec3d n = o + d * t2 - C;
        n = normalize(n);
        ans.position = o + d * t2;
        ans.geometryNormal = n;
        ans.geometryTangent = normalize(Vec3d(n.z, 0, -n.x));
        ans.geometryBitangent = normalize(cross(n, ans.geometryTangent));
        ans.material = material;
        ans.shFrame = Frame(n);
        ans.uv.x = (atan2(n.z, n.x) + M_PI) / M_PI / 2;
        ans.uv.y = acos(n.y) / M_PI;
        ans.object = this;
        flag = true;
    }
    return flag ? std::make_optional(ans) : std::nullopt;
}

std::shared_ptr<Light> Sphere::getLight() const
{
    return lightPtr;
}

void Sphere::setLight(std::shared_ptr<Light> light)
{
    lightPtr = light;
}

double Sphere::area() const
{
    return 4 * M_PI * radius * radius;
}

Intersection Sphere::sample(const Point2d &positionSample) const
{
    Intersection ans;
    Vec3d v = CoordConvertor::cartesian2SphericalVec(positionSample);
    ans.position = center + v * radius;
    ans.geometryNormal = v;
    auto n = v;
    ans.geometryTangent = normalize(Vec3d(n.z, 0, -n.x));
    ans.geometryBitangent = normalize(cross(n, ans.geometryTangent));
    ans.material = material;
    return ans;
}

BoundingBox3f Sphere::WorldBound() const {
	Point3d pMin = center - Vec3d(radius);
	Point3d pMax = center + Vec3d(radius);
	return BoundingBox3f(pMin, pMax);
}

Sphere::Sphere(const Json & json)  :Entity(json){
    center = getOptional(json,"center",Point3d(0,0,0));
    radius = getOptional(json,"radius",1.0);
}

void rtcSphereBoundsFunc(const RTCBoundsFunctionArguments *args)
{
    Sphere *sphere = (Sphere *)args->geometryUserPtr;
    RTCBounds *bounds = args->bounds_o;

    BoundingBox3f box = sphere->WorldBound();
    
    bounds->lower_x = box.pMin.x;
    bounds->lower_y = box.pMin.y;
    bounds->lower_z = box.pMin.z;

    bounds->upper_x = box.pMax.x;
    bounds->upper_y = box.pMax.y;
    bounds->upper_z = box.pMax.z;    
}

void rtcSphereIntersectFunc(const RTCIntersectFunctionNArguments *args)
{
    int *valid = args->valid;
    void *ptr  = args->geometryUserPtr;

    RTCRayHitN *rayhit = args->rayhit;
    RTCRayN *ray = RTCRayHitN_RayN(rayhit, 1);
    RTCHitN *hit = RTCRayHitN_HitN(rayhit, 1);
    unsigned int primID = args->primID;

    Sphere *sphere = (Sphere *) ptr;

    if(!valid[0]) return;

    double ray_ox = RTCRayN_org_x(ray, 1, 0),
           ray_oy = RTCRayN_org_y(ray, 1, 0),
           ray_oz = RTCRayN_org_z(ray, 1, 0),
           ray_dx = RTCRayN_dir_x(ray, 1, 0),
           ray_dy = RTCRayN_dir_y(ray, 1, 0),
           ray_dz = RTCRayN_dir_z(ray, 1, 0);

    Point3d ray_org(ray_ox, ray_oy, ray_oz);
    Vec3d   ray_dir(ray_dx, ray_dy, ray_dz);

    Vec3d v = ray_org - sphere->center;
    const double A = dot(ray_dir, ray_dir);
    const double B = 2.0 * dot(v, ray_dir);
    const double C = dot(v, v) - sphere->radius * sphere->radius;
    const double D = B * B - 4.0f * A * C; 

    if(D < .0) return;
    const double Q = std::sqrt(D);
    const double rcpA = 1.0 / A;
    const double t0 = 0.5 * rcpA * (-B - Q);
    const double t1 = 0.5 * rcpA * (-B + Q);
    RTCHit potentialHit;
    potentialHit.u = potentialHit.v =.0;
    potentialHit.geomID = args->geomID;
    if ((RTCRayN_tnear(ray, 1, 0) < t0) & (t0 < RTCRayN_tfar(ray, 1, 0))) {
        int imask;
        bool mask = 1;
        {
            imask = mask ? -1 : 0;
        }

        Vec3d Ng = ray_org + ray_dir * t0 - sphere->center;
        potentialHit.Ng_x = Ng.x;
        potentialHit.Ng_y = Ng.y;
        potentialHit.Ng_z = Ng.z;

        RTCFilterFunctionNArguments fargs;
        fargs.valid = (int *)&imask;
        fargs.geometryUserPtr = ptr;
        fargs.context = args->context;
        fargs.ray = (RTCRayN *) args->rayhit;
        fargs.hit = (RTCHitN *) &potentialHit;
        fargs.N = 1;

        //* uv = {theta, phi}
        Normal3d normal {Ng.x, Ng.y, Ng.z};
        double theta = std::acos(normal.z);
        double phi = std::atan(normal.y / normal.x);
        if (normal.x < 0) 
            phi += M_PI;
        if (phi < 0)
            phi += 2 * M_PI;
        potentialHit.u = theta;
        potentialHit.v = phi;

        const float old_t = RTCRayN_tfar(ray, 1, 0);
        RTCRayN_tfar(ray, 1, 0) = t0;
        rtcFilterIntersection(args, &fargs);

        if(imask == -1){
            *((RTCHit *)hit) = potentialHit;
        } else {
            RTCRayN_tfar(ray, 1, 0) = old_t;
        }
    }
    if ((RTCRayN_tnear(ray, 1, 0) < t1) & (t1 < RTCRayN_tfar(ray, 1, 0))) {
        int imask;
        bool mask = 1;
        {
            imask = mask ? -1 : 0;
        }
        Vec3d Ng = ray_org + t1 * ray_dir - sphere->center;
        potentialHit.Ng_x = Ng.x;
        potentialHit.Ng_y = Ng.y;
        potentialHit.Ng_z = Ng.z;

        RTCFilterFunctionNArguments fargs;
        fargs.valid = (int *)&imask;
        fargs.geometryUserPtr = ptr;
        fargs.context = args->context;
        fargs.ray = (RTCRayN *) args->rayhit;
        fargs.hit = (RTCHitN *) &potentialHit;
        fargs.N = 1;

        //* uv = {theta, phi}
        Normal3d normal {Ng.x, Ng.y, Ng.z};
        double theta = std::acos(normal.z);
        double phi = std::atan(normal.y / normal.x);
        if (normal.x < 0) 
            phi += M_PI;
        if (phi < 0)
            phi += 2 * M_PI;
        potentialHit.u = theta;
        potentialHit.v = phi;

        const float old_t = RTCRayN_tfar(ray, 1, 0);
        RTCRayN_tfar(ray, 1, 0) = t1;
        rtcFilterIntersection(args, &fargs);

        if(imask == -1){
            // when in occludedFunc
            //RTCRayN_tfar(ray, 1, 0) = -std::numeric_limits<float>::infinity();
            *((RTCHit *)hit) = potentialHit;
        } else {
            RTCRayN_tfar(ray, 1, 0) = old_t;
        }
    }
}

//todo
void rtcSphereOccludeFunc(const RTCOccludedFunctionNArguments *args) {
    return;
}
/*
RTCGeometry Sphere::toEmbreeGeometry(RTCDevice device) const 
{
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryUserPrimitiveCount(geom, 1);
    rtcSetGeometryUserData(geom, (void *)this);
    rtcSetGeometryBoundsFunction(geom, rtcSphereBoundsFunc, nullptr);
    rtcSetGeometryIntersectFunction(geom,rtcSphereIntersectFunc);
    rtcSetGeometryOccludedFunction(geom, rtcSphereOccludeFunc);
    rtcCommitGeometry(geom);
    return geom;
}
*/
EntitySurfaceInfo Sphere::getEntitySurfaceInfo(int instID, Point2d uv) const{
    auto [theta, phi] = uv;
    Normal3d normal {
        std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta)
    };
    
    return {
        center + normal * radius,
        normal,
        Normal3d(), Normal3d(),
        {theta * INV_PI, phi * 0.5 * INV_PI}
    };
}
