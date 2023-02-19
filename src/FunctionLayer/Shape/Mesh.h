/**
 * @file Mesh.h
 * @author Chenxi Zhou
 * @brief Triangle mesh only
 * @version 0.1
 * @date 2022-08-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <optional>
#include <memory>
#include "Entity.h"
#include "Triangle.h"
#include "ResourceLayer/File/MeshData.h"
#include "FunctionLayer/Acceleration/Bvh.h"

class TraceableMesh;
//! Only triangle mesh
class Mesh : public Entity
{
public:
    friend class Bvh;

    Mesh() = default;

    Mesh(std::shared_ptr<MeshData> _data, std::shared_ptr<Material> _material,const Json & json );

    virtual ~Mesh() = default;

    virtual std::optional<Intersection> intersect(const Ray &r) const override;

    virtual double area() const override;

    virtual Intersection sample(const Point2d &positionSample) const override;

    virtual BoundingBox3f WorldBound() const override;

    Triangle getTriangle(int idx) const;

    virtual RTCGeometry toEmbreeGeometry(RTCDevice device) const override;

    virtual std::optional<Intersection> getIntersectionFromRayHit(const UserRayHit1 &rayhit) const override;

protected:
    std::shared_ptr<TraceableMesh> meshData;
    std::shared_ptr<TransformMatrix3D> inverseMatrix;
    BoundingBox3f m_aabb;

    //not used
    Eigen::MatrixXd m_vertices;
    std::vector<Point3i> m_indices;

    virtual void apply() override;
};