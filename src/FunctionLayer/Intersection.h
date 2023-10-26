/**
 * @file Intersection.h
 * @author orbitchen
 * @brief Intersection information on an object.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <memory>
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Geometry/Frame.h"
#include "CoreLayer/Ray/Ray.h"
#include "FunctionLayer/Material/Material.h"

class Entity;
class Material;
class Medium;
//* Add t in intersection, by zcx 8-22
struct Intersection {
    /// @brief t indicatees that current position = last ray's origin + t * last ray's direction.
    ///		   This domain is helpful for medium scattering.
    double t;

    Point3d position;
    Normal3d geometryNormal;
    Normal3d geometryTangent;
    Normal3d geometryBitangent;
    Point2d uv;

    // shadingFrame
    Frame shFrame;

    Vec3d dpdu, dpdv;
    Normal3d dndu, dndv;

    // raydifferential
    float dudx, dvdx, dudy, dvdy;
    Vec3d dpdx, dpdy;

    // std::shared_ptr<Entity> object;
    const Entity *object;

    /// @brief Material of intersection point. If material!=nullptr, the intersection event is a surface scattering event.
    std::shared_ptr<Material> material;

    /// @brief Medium of scattering point. If medium!=nullptr, the intersection event is a medium scattering event.
    std::shared_ptr<Medium> medium;

    Vec3d toLocal(const Vec3d &d) const {
        return shFrame.toLocal(d);
    }

    Vec3d toWorld(const Vec3d &d) const {
        return shFrame.toWorld(d);
    }

    Ray spawnRay(const Point3d &target) const {
        Vec3d dir = (target - position);
        double l = dir.length();
        const double eps = 1e-4;
        return Ray(position, normalize(dir), eps, l);
    }
    // compute differential, reference implementation in lite
    void computeRayDifferential(const Ray &ray) {

        dudx = 0.f;
        dudy = 0.f;
        dvdx = 0.f;
        dvdy = 1.f;
        dpdx = Vec3d(0.f);
        dpdy = Vec3d(0.f);

        Point3d p = position;
        Vec3d n = geometryNormal;
        Vec3d ox = Vec3d{ray.origin_x[0], ray.origin_x[1], ray.origin_x[2]};
        Vec3d oy = Vec3d{ray.origin_y[0], ray.origin_y[1], ray.origin_y[2]};

        double distance = dot(n, Vec3d(p[0], p[1], p[2]));
        double tx = (distance - dot(n, ox)) / (dot(n, ray.direction_x));
        if (std::isinf(tx) || std::isnan(tx))
            return;
        double ty = (distance - dot(n, oy)) / (dot(n, ray.direction_y));
        if (std::isinf(ty) || std::isnan(ty))
            return;

        dpdx = normalize((ray.origin_x + tx * ray.direction_x) - p);
        dpdy = normalize((ray.origin_y + ty * ray.direction_y) - p);

        int dim[2];
        if (std::abs(n[0]) > std::abs(n[1]) && std::abs(n[0]) > std::abs(n[2])) {
            dim[0] = 1;
            dim[1] = 2;
        } else if (std::abs(n[1]) > std::abs(n[2])) {
            dim[0] = 0;
            dim[1] = 2;
        } else {
            dim[0] = 0;
            dim[1] = 1;
        }

        float A[2][2] = {{(float)dpdu[dim[0]], (float)dpdv[dim[0]]},
                         {(float)dpdu[dim[1]], (float)dpdv[dim[1]]}};
        float Bx[2] = {(float)dpdx[dim[0]], (float)dpdx[dim[1]]};
        float By[2] = {(float)dpdy[dim[0]], (float)dpdy[dim[1]]};

        auto solveLinearSystem2x2 = [](const float A[2][2], const float B[2],
                                       float *x0, float *x1) {
            float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
            if (std::abs(det) < 1e-10f)
                return false;
            *x0 = (A[1][1] * B[0] - A[0][1] * B[1]) / det;
            *x1 = (A[0][0] * B[1] - A[1][0] * B[0]) / det;
            if (std::isnan(*x0) || std::isnan(*x1))
                return false;
            return true;
        };

        if (!solveLinearSystem2x2(A, Bx, &dudx, &dvdx))
            dudx = dvdx = .0f;
        if (!solveLinearSystem2x2(A, Bx, &dudy, &dvdy))
            dudy = dvdy = .0f;

        return;
    }
};
