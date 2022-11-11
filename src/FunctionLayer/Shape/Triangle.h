/**
 * @file Triangle.h
 * @author Pengpei Hong
 * @brief Triangle implementation, transform not implemented yet
 * @version 0.1
 * @date 2022-06-26
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma once

#include <optional>
#include "Entity.h"

class TriangleMesh{
public:
	const int nTriangles;
	const int nVertices;
	std::shared_ptr<std::vector<int>> vertexIndices;//the i-th triangle maps vertex[i * 3], vertex[i * 3 + 1], vertex[i * 3 + 2]
	std::shared_ptr<std::vector<Point3d>> p; //geometry position
	std::shared_ptr<std::vector<Normal3d>> n; //shading normal
	std::shared_ptr<std::vector<Vec3d>> s; //shading tangent(optional)
	std::shared_ptr<std::vector<Point2d>> uv; //uv coordinates
	std::shared_ptr<Material> material;
	/**
	 @brief constructor of TriangleMesh, parameter pointers(except material) will point to nullptr after construction
	 @param <_nTriangles> number of triangle
	 @param <_nVertices> number of vertices(equal to the sizeof of _p)
	 @param <_p> position of vertices
	 @param <_n> shading normal of vertices(optional)
	 @param <_s> shading tangent of vertices(optional)
	 @param <_uv> uv coordinates of vertices(optional)
	 @param <_material> material of triangle mesh
	*/
	TriangleMesh(const int& _nTriangles, const int& _nVertices, 
	             const std::shared_ptr<std::vector<int>>& _vertexIndices, 
				 const std::shared_ptr<std::vector<Point3d>>& _p, 
				 const std::shared_ptr<std::vector<Normal3d>>& _n, 
				 const std::shared_ptr<std::vector<Vec3d>>& _s, 
				 const std::shared_ptr<std::vector<Point2d>>& _uv, 
				 const std::shared_ptr<Material>& _material);
};

//vertices in counter-clockwise order
class Triangle: public Entity{
protected:
	int vertexId[3];
	int faceId;
	std::shared_ptr<TriangleMesh> mesh;
public:
	virtual void apply() override;
	Triangle(const std::shared_ptr<TriangleMesh>& _mesh, const int& _faceId);
	Triangle(const std::vector<Point3d>& points, const std::shared_ptr<Material>& _material);
	virtual std::optional<Intersection> intersect(const Ray& r) const override;
	virtual double area() const override;
	virtual Intersection sample(const Point2d& positionSample) const override;

	virtual BoundingBox3f WorldBound() const override;


	//TODO Delete this
	virtual RTCGeometry toEmbreeGeometry(RTCDevice device) const override {
		RTCGeometry geom;
		return geom;
	}
};