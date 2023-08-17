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
#include "CoreLayer/Geometry/Angle.h"

Sphere::Sphere(Point3d _center, double _radius, std::shared_ptr<Material> _material)
    : center(_center), radius(_radius) {
    this->material = _material;
}

void Sphere::apply() {
}

std::optional<Intersection> Sphere::intersect(const Ray &r) const {
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
    if (t1 >= 0 && t1 < t && t1 >= r.timeMin && t1 <= r.timeMax) {
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
    if (t2 >= 0 && t2 < t && t2 >= r.timeMin && t2 <= r.timeMax) {
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
    ans.t = t;
    return flag ? std::make_optional(ans) : std::nullopt;
}

double Sphere::area() const {
    return 4 * M_PI * radius * radius;
}

Intersection Sphere::sample(const Point2d &positionSample) const {
    Intersection ans;
    Polar3d polar = Polar3d(1.0, Angle(positionSample.x * 2 * M_PI, Angle::EAngleType::ANGLE_RAD), Angle(acos(positionSample.y * 2.0 - 1.0), Angle::EAngleType::ANGLE_RAD));
    Vec3d v = polar.toVec3d();
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

Sphere::Sphere(const Json &json) : Entity(json) {
    center = getOptional(json, "center", Point3d(0, 0, 0));
    radius = getOptional(json, "radius", 1.0);
}
