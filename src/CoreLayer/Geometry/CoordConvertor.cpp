/**
 * @file CoordConvertor.cpp
 * @author Zhimin Fan
 * @brief Provide convertor between standard coordinates.
 * @version 0.1
 * @date 2022-05-27
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "CoordConvertor.h"
#include <cmath>

Point3d CoordConvertor::cartesian2Spherical(const Point2d &p)
{
    double phi = p.x * 2 * 3.14159;
    double theta = acos(p.y * 2 - 1);
    double cosPhi = cos(phi);
    double sinPhi = sin(phi);
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);
    return Point3d(sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
}

Vec3d CoordConvertor::cartesian2SphericalVec(const Point2d &p)
{
    Point3d ans = cartesian2Spherical(p);
    return Vec3d(ans.x, ans.y, ans.z);
}

Vec3d CoordConvertor::world2TBN(const Vec3d &v, const Vec3d &t, const Vec3d &b, const Vec3d &n)
{
    return Vec3d(
        v.x * t.x + v.y * t.y + v.z * t.z,
        v.x * b.x + v.y * b.y + v.z * b.z,
        v.x * n.x + v.y * n.y + v.z * n.z);
}

Vec3d CoordConvertor::TBN2World(const Vec3d &v, const Vec3d &t, const Vec3d &b, const Vec3d &n)
{
    return Vec3d(
        v.x * t.x + v.y * b.x + v.z * n.x,
        v.x * t.y + v.y * b.y + v.z * n.y,
        v.x * t.z + v.y * b.z + v.z * n.z);
}
