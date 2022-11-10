/**
 * @file Triangle.cpp
 * @author Pengpei Hong
 * @brief Triangle implementation, transform not implemented yet
 * @version 0.1
 * @date 2022-06-26
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#include "Triangle.h"
#include "FunctionLayer/Intersection.h"

TriangleMesh::TriangleMesh(const int& _nTriangles, const int& _nVertices,
	const std::shared_ptr<std::vector<int>>& _vertexIndices, const std::shared_ptr<std::vector<Point3d>>& _p,
	const std::shared_ptr<std::vector<Normal3d>>& _n, const std::shared_ptr<std::vector<Vec3d>>& _s,
	const std::shared_ptr<std::vector<Point2d>>& _uv, const std::shared_ptr<Material>& _material) :
	nTriangles(_nTriangles), nVertices(_nVertices), vertexIndices(std::move(_vertexIndices)), p(_p), n(_n), s(_s), uv(_uv), material(_material){
}
Triangle::Triangle(const std::shared_ptr<TriangleMesh>& _mesh, const int& _faceId): mesh(_mesh), faceId(_faceId){
	vertexId[0] = _mesh->vertexIndices->at(_faceId * 3);
	vertexId[1] = _mesh->vertexIndices->at(_faceId * 3 + 1);
	vertexId[2] = _mesh->vertexIndices->at(_faceId * 3 + 2);
	material = mesh->material;
}
Triangle::Triangle(const std::vector<Point3d>& points, const std::shared_ptr<Material>& _material){
	assert(points.size() >= 3);
	std::shared_ptr<std::vector<Point3d>> p = std::make_shared<std::vector<Point3d>>(points);
	std::shared_ptr<std::vector<int>> vertexIndices = std::make_shared<std::vector<int>>(std::vector<int>({ 0, 1, 2 }));
	mesh = std::make_shared<TriangleMesh>(1, 3, vertexIndices, p, nullptr, nullptr, nullptr, material);
	material = _material;
	for (int i = 0; i < 3; i++) vertexId[i] = i;
}
void Triangle::apply()
{
}
//@brief a simple version using barycentric coordinates to calculate the intersection
//does not handle the case where ray and triangle fall in the same plane
std::optional<Intersection> Triangle::intersect(const Ray& r) const {

	Point3d p[3];
	for (int i = 0; i < 3; i++) p[i] = mesh->p->at(i);

	//if parallel return nullptr
	Normal3d geometryNormal = normalize(cross(p[1] - p[0], p[2] - p[0]));
	if (fabs(dot(geometryNormal, r.direction)) < 1e-8) return std::nullopt;

	//calculate barycentric coordinates(solve a system of three-variable linear equations): mul(A, X) = Y, based on Cramer's Rule
	//X[0] is alpha, X[1] is beta, X[2] is time
	std::vector<std::vector<double>> A(3, std::vector<double>(3)), transposeCofactorA(A);
	std::vector<double> X(3), Y(3);
	for (int i = 0; i < 3; i++) {
		A[i][0] = p[0][i] - p[2][i];
		A[i][1] = p[1][i] - p[2][i];
		A[i][2] = -r.direction[i];
		Y[i] = r.origin[i] - p[2][i];
	}
	//transpose of cofactor
	for (int col = 0; col < 3; col++) {
		for (int row = 0; row < 3; row++) {
			auto mod3 = [&](const int& v) {
				return v >= 3 ? v - 3 : v;
			};
			transposeCofactorA[col][row] = A[mod3(row + 1)][mod3(col + 1)] * A[mod3(row + 2)][mod3(col + 2)] - A[mod3(row + 1)][mod3(col + 2)] * A[mod3(row + 2)][mod3(col + 1)];
		}
	}

	//calculate X
	double det = A[0][0] * transposeCofactorA[0][0] + A[0][1] * transposeCofactorA[1][0] + A[0][2] * transposeCofactorA[2][0], invDet = 1.0 / det;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			X[i] += transposeCofactorA[i][j] * Y[j];
		}
		X[i] *= invDet;
	}
	if (!(X[0] >= 0 && X[0] <= 1) || !(X[1] >= 0 && X[1] <= 1) || !(X[0] + X[1] <= 1) || !(X[2] >= r.timeMin && X[2] <= r.timeMax)) return std::nullopt;

	//calculate dpdu, dpdv
	auto getUVs = [&](Point2d uv[]) {
		if (mesh->uv) {
			for (int i = 0; i < 3; i++) uv[i] = mesh->uv->at(vertexId[i]);
		}
		else {//default settings
			uv[0] = Point2d(0, 0);
			uv[1] = Point2d(1, 0);
			uv[2] = Point2d(1, 1);
		}
	};
	Vec3d dpdu, dpdv;
	Point2d uv[3];
	getUVs(uv);
	Vec2d duv02 = uv[0] - uv[2], duv12 = uv[1] - uv[2];
	Vec3d dp02 = p[0] - p[2], dp12 = p[1] - p[2];
	double uvDet = duv02[0] * duv12[1] - duv02[1] * duv12[0];
	if (fabs(uvDet) < 1e-8) {
		coordinateSystem(normalize(geometryNormal), dpdu, dpdv);
	}
	else {
		double invDet = 1.0 / uvDet;
		dpdu = (duv12[1] * dp02 - duv02[1] * dp12) * invDet;
		dpdv = (-duv12[0] * dp02 + duv02[0] * dp12) * invDet;
	}

	//get intersection, mostly refer to PBRT-V3
	Intersection its;
	its.position = r.origin + r.direction * X[2];
	its.t = (its.position - r.origin).length();		//* caculate t, by zcx 8-22
	its.geometryNormal = geometryNormal;
	coordinateSystem(geometryNormal, its.geometryTangent, its.geometryBitangent);
	its.uv = uv[0] * X[0] + uv[1] * X[1] + uv[2] * (1 - X[0] - X[1]);
	its.dpdu = dpdu, its.dpdv = dpdv;
	its.shFrame = Frame(geometryNormal);
	its.material = material;
	its.object = this;
	if (mesh->n || mesh->s) {
		Normal3d shadingNormal;
		if (mesh->n) {
			shadingNormal = mesh->n->at(vertexId[0]) * X[0] + mesh->n->at(vertexId[1]) * X[1] + mesh->n->at(vertexId[2]) * (1 - X[0] - X[1]);
			if (shadingNormal.length2() > 0) shadingNormal = normalize(shadingNormal);
			else shadingNormal = geometryNormal;
		}
		else shadingNormal = geometryNormal;

		Normal3d shadingTangent;
		if (mesh->s) {
			shadingTangent = mesh->s->at(vertexId[0]) * X[0] + mesh->s->at(vertexId[1]) * X[1] + mesh->s->at(vertexId[2]) * (1 - X[0] - X[1]);
			if (shadingTangent.length2() > 0) shadingTangent = normalize(shadingTangent);
			else shadingTangent = normalize(its.dpdu);
		}
		else shadingTangent = normalize(its.dpdu);

		Normal3d shadingBitangent = cross(shadingTangent, shadingNormal);
		if (shadingBitangent.length2() > 0) shadingBitangent = normalize(shadingBitangent), shadingTangent = cross(shadingBitangent, shadingNormal);
		else coordinateSystem(shadingNormal, shadingTangent, shadingBitangent);
		its.shFrame = Frame(shadingTangent, shadingBitangent, shadingNormal);

		Normal3d dndu, dndv;
		if (mesh->n) {
			Vec2d duv02 = uv[0] - uv[2], duv12 = uv[1] - uv[2];
			Normal3d dn02 = mesh->n->at(vertexId[0]) - mesh->n->at(vertexId[2]), dn12 = mesh->n->at(vertexId[1]) - mesh->n->at(vertexId[2]);
			double uvDet = duv02[0] * duv12[1] - duv02[1] * duv12[0];
			if (fabs(uvDet) < 1e-8) {
				Vec3d dn = cross(Vec3d(mesh->n->at(vertexId[2]) - mesh->n->at(vertexId[0])),
					Vec3d(mesh->n->at(vertexId[1]) - mesh->n->at(vertexId[0])));
				if (dn.length2() == 0)
					dndu = dndv = Normal3d(0, 0, 0);
				else {
					Vec3d dnu, dnv;
					coordinateSystem(dn, dnu, dnv);
					dndu = Normal3d(dnu);
					dndv = Normal3d(dnv);
				}
			}
			else {
				dndu = dndv = Normal3d(0, 0, 0);
			}
		}
		else dndu = dndv = Normal3d(0, 0, 0);
		its.dndu = dndu;
		its.dndv = dndv;
	}
	else its.dndu = its.dndv = Normal3d(0, 0, 0);
	return std::make_optional(its);
}
Intersection Triangle::sample(const Point2d& positionSample) const{
	double b[3];
	b[0] = positionSample[0], b[1] = positionSample[1], b[2] = 1 - b[0] - b[1];
	Point3d p[3];
	Normal3d n[3];
	for (int i = 0; i < 3; i++) p[i] = mesh->p->at(vertexId[i]), n[i] = mesh->n->at(vertexId[i]);
	Normal3d geometryNormal = normalize(cross(p[1] - p[0], p[2] - p[0]));
	Intersection its;
	its.position = p[0] * b[0] + p[1] * b[1] + p[2] * b[2];
	its.geometryNormal = geometryNormal;
	if (mesh->n) {//make the normal forward
		Normal3d shadingNormal = normalize(n[0] * b[0] + n[1] * b[1] + n[2] * b[2]);
		if (dot(geometryNormal, shadingNormal) < 0) its.geometryNormal = -its.geometryNormal;
	}
	return its;
}

double Triangle::area() const
{
	const Point3d& p0 = mesh->p->at(vertexId[0]);
	const Point3d& p1 = mesh->p->at(vertexId[1]);
	const Point3d& p2 = mesh->p->at(vertexId[2]);
	return 0.5 * cross(p1 - p0, p2 - p0).length();
}
BoundingBox3f Triangle::WorldBound() const{
	BoundingBox3f result;
	for(int i = 0; i < 3; i++) result = BoundingBoxUnion(result, BoundingBox3f(mesh->p->at(vertexId[i])));
	return result;
}

