/**
 * @file  Frame.h
 * @author Junping Yuan
 * @brief
 * @version 0.1
 * @date  2022/06/06
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma once

#include "Geometry.h"
#include "CoreLayer/Math/Common.h"


static void coordinateSystem(const Normal3d &a, Vec3d  &b, Vec3d  &c) {
    if (std::abs(a.x) > std::abs(a.y)) {
        float invLen = 1.0f / std::sqrt(a.x * a.x + a.z * a.z);
        c = Vec3d (a.z * invLen, 0.0f, -a.x * invLen);
    } else {
        float invLen = 1.0f / std::sqrt(a.y * a.y + a.z * a.z);
        c = Vec3d(0.0f, a.z * invLen, -a.y * invLen);
    }
    Vec3d  _a(a.x,a.y,a.z);
    b = cross(_a,c);
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

    /** \brief Assuming that the given direction is in the local coordinate
     * system, return the cosine of the angle between the normal and v */
    static float cosTheta(const Vec3d &v) {
        return v.z;
    }

    /** \brief Assuming that the given direction is in the local coordinate
     * system, return the sine of the angle between the normal and v */
    static float sinTheta(const Vec3d &v) {
        float temp = sinTheta2(v);
        if (temp <= 0.0f)
            return 0.0f;
        return std::sqrt(temp);
    }

    /** \brief Assuming that the given direction is in the local coordinate
     * system, return the tangent of the angle between the normal and v */
    static float tanTheta(const Vec3d &v) {
        float temp = 1 - v.z*v.z;
        if (temp <= 0.0f)
            return 0.0f;
        return std::sqrt(temp) / v.z;
    }

    /** \brief Assuming that the given direction is in the local coordinate
     * system, return the Squared  sine of the angle between the normal and v */
    static float sinTheta2(const Vec3d &v) {
        return 1.0f - v.z * v.z;
    }

    /** \brief Assuming that the given direction is in the local coordinate
     * system, return the sine of the phi parameter in spherical coordinates */
    static float sinPhi(const Vec3d &v) {
        float sinTheta = Frame::sinTheta(v);
        if (sinTheta == 0.0f)
            return 1.0f;
        return clamp(v.y / sinTheta, -1.0f, 1.0f);
    }

    /** \brief Assuming that the given direction is in the local coordinate
     * system, return the cosine of the phi parameter in spherical coordinates */
    static float cosPhi(const Vec3d &v) {
        float sinTheta = Frame::sinTheta(v);
        if (sinTheta == 0.0f)
            return 1.0f;
        return clamp(v.x / sinTheta, -1.0f, 1.0f);
    }

    /** \brief Assuming that the given direction is in the local coordinate
     * system, return the Squared  sine of the phi parameter in  spherical
     * coordinates */
    static float sinPhi2(const Vec3d &v) {
        return clamp(v.y * v.y / sinTheta2(v), 0.0f, 1.0f);
    }

    /** \brief Assuming that the given direction is in the local coordinate
     * system, return the Squared  cosine of the phi parameter in  spherical
     * coordinates */
    static float cosPhi2(const Vec3d &v) {
        return clamp(v.x * v.x / sinTheta2(v), 0.0f, 1.0f);
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
