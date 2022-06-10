/**
 * @file Sphere.cpp
 * @author Zhimin Fan
 * @brief Sphere. Only for test. Ignore transform.
 * @version 0.1
 * @date 2022-05-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Sphere.h"
#include "../Core/FunctionLayer/Intersection.h"
#include "../Geometry/CoordConvertor.h"

Sphere::Sphere(Point3d center, double radius, std::shared_ptr<Material> material)
    : center(center), radius(radius), material(material)
{
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
    double delta = b * b - 4 * a * c + 1e-3;
    if (delta < 0)
        return std::nullopt;
    double sqrtDelta = sqrt(delta);
    double t1 = (-b + sqrtDelta) / (2.0 * a);
    double t2 = (-b - sqrtDelta) / (2.0 * a);
    bool flag = false;
    Intersection ans;
    double t = INFINITY;
    if (t1 >= 0 && t1 < t)
    {
        t = t1;
        Vec3d n = o + d * t2 - C;
        n = normalize(n);
        ans.position = o + d * t1;
        ans.geometryNormal = n;
        ans.geometryTangent = normalize(Vec3d(n.z, 0, -n.x));
        ans.geometryBitangent = normalize(cross(n, ans.geometryTangent));
        ans.material = material;
        ans.shFrame = Frame(n);
        ans.uv.x = (atan2(n.y, n.x) + M_PI) / M_PI / 2;
        ans.uv.y = acos(n.y) / M_PI;
        flag = true;
    }
    if (t2 >= 0 && t2 < t)
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
        flag = true;
    }
    return flag ? std::make_optional(ans) : std::nullopt;
}

std::shared_ptr<Light> Sphere::getLight()
{
    return lightPtr;
}

void Sphere::setLight(std::shared_ptr<Light> light)
{
    lightPtr = light;
}

double Sphere::area() const
{
    return 4 * M_PI * radius;
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
