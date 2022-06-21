/**
 * @file CoordConvertor.h
 * @author Zhimin Fan
 * @brief Provide convertor between standard coordinates.
 * @version 0.1
 * @date 2022-05-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Geometry.h"

class CoordConvertor
{
public:
    // @brief Convert [0,1]^2 to a point on unit sphere.
    static Point3d cartesian2Spherical(const Point2d &p);
    static Vec3d cartesian2SphericalVec(const Point2d &p);
    static Vec3d world2TBN(const Vec3d &v, const Vec3d &t, const Vec3d &b, const Vec3d &n);
    static Vec3d TBN2World(const Vec3d &v, const Vec3d &t, const Vec3d &b, const Vec3d &n);
};