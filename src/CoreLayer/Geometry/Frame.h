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
#include "FastMath.h"

static void coordinateSystem(const Normal3d &normal, Vec3d  &tangent, Vec3d  &bitangent) {
   double sign = copysignf(1.0f, normal.z);
   const double a = -1.0f / (sign + normal.z);
   const double b = normal.x * normal.y * a;
   tangent = Vec3d(1.0f + sign * normal.x * normal.x * a, sign * b, -sign * normal.x);
   bitangent = Vec3d(b, sign + normal.y * normal.y * a, -normal.y);
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
   static double cosTheta(const Vec3d &v) {
       return v.z;
   }

   /** \brief Assuming that the given direction is in the local coordinate
    * system, return the sine of the angle between the normal and v */
   static double sinTheta(const Vec3d &v) {
       double temp = sinTheta2(v);
       if (temp <= 0.0f)
           return 0.0f;
       return fm::sqrt(temp);
   }

   /** \brief Assuming that the given direction is in the local coordinate
    * system, return the tangent of the angle between the normal and v */
   static double tanTheta(const Vec3d &v) {
       double temp = 1 - v.z*v.z;
       if (temp <= 0.0f)
           return 0.0f;
       return fm::sqrt(temp) / v.z;
   }

   /** \brief Assuming that the given direction is in the local coordinate
    * system, return the Squared  sine of the angle between the normal and v */
   static double sinTheta2(const Vec3d &v) {
       return 1.0f - v.z * v.z;
   }

   /** \brief Assuming that the given direction is in the local coordinate
    * system, return the sine of the phi parameter in spherical coordinates */
   static double sinPhi(const Vec3d &v) {
       double sinTheta = Frame::sinTheta(v);
       if (sinTheta == 0.0f)
           return 1.0;
       return clamp(v.y / sinTheta, -1.0, 1.0);
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

   static Frame FromZ(Normal3d z) {
       Vec3d x, y;
       coordinateSystem(z, x, y);
       return Frame(x,y,z);
   }

   static Frame FromX(Normal3d x) {
       Vec3d y, z;
       coordinateSystem(x, y, z);
       return Frame(x, y, z);
   }

   static Frame FromY(Normal3d y) {
       Vec3d x, z;
       coordinateSystem(y, z, x);
       return Frame(x, y, z);
   }
};
