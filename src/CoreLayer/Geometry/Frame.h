/**
 * @file  Frame.h
 * @author Junping Yuan
 * @brief
 * @version 0.1
 * @date  2022/06/06
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumetnormal.com
 *
 */

#pragma once

#include "Geometry.h"
#include "CoreLayer/Math/Common.h"


static void coordinateSystem(const Normal3d &normal, Vec3d  &tangent, Vec3d  &bitangent) {
    double sign = copysignf(1.0f, normal.z);
    const double a = -1.0f/(sign + normal.z);
    const double b = normal.x *normal.y*a;
    tangent = Vec3d(1.0f + sign*normal.x*normal.x*a, sign*b, -sign*normal.x);
    bitangent = Vec3d(b, sign + normal.y*normal.y*a, -normal.y);
}

/// \ingroup Geometry
/// \brief Convert the vector between world coordinate and local coordinate 
struct Frame {
    Vec3d s, t;
    Normal3d  n;

    /// Default constructor -- performs no initialization!
    Frame() { }

    /// Given a normal and tangent vectors, construct a new coordinate frame
    Frame(const Vec3d  &s, const Vec3d &t, const Normal3d  &n)
            : s(s), t(t), n(n) { }

    /// Construct a frame from the given orthonormal vectors
    Frame(const Vec3d &x, const Vec3d &y, const Vec3d &z)
            : s(x), t(y), n(z.x,z.y,z.z) { }

    /// Construct a new coordinate frame from a single vector
    Frame(const Normal3d n) : n(n){
        coordinateSystem(n, s, t);
    }

    /// Convert from world coordinates to local coordinates
    Vec3d toLocal(const Vec3d &v) const {
        return Vec3d(
                dot(v,s), dot(v,t), dot(v,n)
        );
    }

    /// Convert from local coordinates to world coordinates
    Vec3d toWorld(const Vec3d &v) const {
        return s * v.x + t * v.y   + n * v.z ;
    }




    /// Equality test
    bool operator==(const Frame &frame) const {
        return frame.s == s && frame.t == t && frame.n == n;
    }

    /// Inequality test
    bool operator!=(const Frame &frame) const {
        return !operator==(frame);
    }

    static Vec3d reflect(const Vec3d & w){
        return Vec3d(-w.x,-w.y,w.z);
    }

    static Vec3d reflect(const Vec3d &wo, const Vec3d & n) {
        return -wo + 2 * dot(wo, n) * n;
    }
};
