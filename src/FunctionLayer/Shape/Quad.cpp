#include "Quad.h"
#include "FunctionLayer/Intersection.h"
#include "FastMath.h"

Quad::Quad(const Json &json) : Entity(json) {
    _base = getOptional(json, "base", Point3d(0, 0, 0));
    _edge0 = getOptional(json, "edge0", Vec3d(1, 0, 0));
    _edge1 = getOptional(json, "edge1", Vec3d(0, 0, 1));

    _base = matrix->operator*(_base);
    _edge0 = matrix->operator*(_edge0);
    _edge1 = matrix->operator*(_edge1);

    _base -= _edge0 * 0.5f;
    _base -= _edge1 * 0.5f;
}

std::optional<Intersection> Quad::intersect(const Ray &r) const {
    Vec3d n = normalize(cross(_edge1, _edge0));

    double dotW = dot(r.direction, n);
    if (fm::abs(dotW) < EPSILON) {
        return std::nullopt;
    }
    double t = dot(n, (_base - r.origin)) / dotW;

    if (t < r.timeMin || t > r.timeMax)
        return std::nullopt;

    Point3d q = r.at(t);
    Vec3d v = q - _base;
    double l0 = dot(v, _edge0) * _invSq.x;
    double l1 = dot(v, _edge1) * _invSq.y;

    if (l0 < 0.0f || l0 > 1.0f || l1 < 0.0f || l1 > 1.0f)
        return std::nullopt;

    Intersection ans;
    ans.t = t;
    ans.position = r.at(t);
    ans.geometryNormal = n;
    ans.geometryTangent = normalize(Vec3d(n.z, 0, -n.x));
    ans.geometryBitangent = normalize(cross(n, ans.geometryTangent));
    ans.material = material;
    ans.shFrame = Frame(n);
    ans.uv.x = l0;
    ans.uv.y = l1;
    ans.object = this;

    return std::make_optional(ans);
}

double Quad::area() const {
    return _area;
}

Intersection Quad::sample(const Point2d &positionSample) const {
    Intersection its;
    its.uv = positionSample;
    its.position = _base + _edge0 * positionSample.x + _edge1 * positionSample.y;
    its.geometryNormal = normalize(cross(_edge1, _edge0));
    return its;
}

BoundingBox3f Quad::WorldBound() const {
    BoundingBox3f bb;
    bb = BoundingBoxPointUnion(bb, _base);
    bb = BoundingBoxPointUnion(bb, _base + _edge0);
    bb = BoundingBoxPointUnion(bb, _base + _edge1);
    bb = BoundingBoxPointUnion(bb, _base + _edge0 + _edge1);

    return bb;
}

void Quad::apply() {

    //    _base = matrix->operator *(_base);
    //    _edge0 = matrix->operator *(_edge0);
    //    _edge1 = matrix->operator *(_edge1);
    //    _base -= _edge0*0.5;
    //    _base -= _edge1*0.5;

    _area = cross(_edge0, _edge1).length();
    _invArea = 1 / _area;
    _invSq = Vec2d(1 / _edge0.length2(), 1 / _edge1.length2());
}
