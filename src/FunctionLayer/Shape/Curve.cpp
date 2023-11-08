#include "Curve.h"
#include "ResourceLayer/File/CurveIO.h"
#include "FunctionLayer/Intersection.h"
#include "CoreLayer/Geometry/BoundingBox.h"

template<typename T>
inline T quadraticDeriv(T p0, T p1, T p2, T p3, double t) {

    const double t0 = 1 - t, t1 = t;
    const double B0 = -(t0 * t0);
    const double B1 = -2 * (t0 * t1) + (t0 * t0);
    const double B2 = +2 * (t0 * t1) - (t1 * t1);
    const double B3 = +(t1 * t1);
    return 3.0 * (B0 * p0 + B1 * p1 + B2 * p2 + B3 * p3);
}

template<typename T>
inline T quadraticDeriv(T p0, T p1, T p2, double t) {
    return (p0 - p1 * 2.0 + p2) * t + (p1 - p0);
}

template<typename T>
inline T quadratic(T p0, T p1, T p2, double t) {
    return (0.5 * t * t - t + 0.5) * p0 + (t - t * t + 0.5) * p1 + 0.5 * t * t * p2;
}

template<typename T>
inline T quadratic(T p0, T p1, T p2, T p3, double t) {
    return quadratic(lerp(p0, p1, t), lerp(p1, p2, t), lerp(p2, p3, t), t);
}

inline Vec2d quadraticMinMax(double p0, double p1, double p2) {
    double xMin = (p0 + p1) * 0.5;
    double xMax = (p1 + p2) * 0.5;
    if (xMin > xMax)
        std::swap(xMin, xMax);

    double tFlat = (p0 - p1) / (p0 - 2 * p1 + p2);
    if (tFlat > 0 && tFlat < 1) {
        double xFlat = quadratic(p0, p1, p2, tFlat);
        xMin = std::min(xMin, xFlat);
        xMax = std::max(xMax, xFlat);
    }
    return {xMin, xMax};
}

static BoundingBox3f curveBox(const Vec4d &q0, const Vec4d &q1, const Vec4d &q2) {
    Vec2d xMinMax(quadraticMinMax(q0[0], q1[0], q2[0]));
    Vec2d yMinMax(quadraticMinMax(q0[1], q1[1], q2[1]));
    Vec2d zMinMax(quadraticMinMax(q0[2], q1[2], q2[2]));
    double maxW = std::max(q0[3], std::max(q1[3], q2[3]));
    return {
        Point3d(xMinMax.x, yMinMax.x, zMinMax.x) - Vec3d(maxW),
        Point3d(xMinMax.y, yMinMax.y, zMinMax.y) + Vec3d(maxW)};
}

static Vec4d project(const Vec3d &o, const Vec3d &lx, const Vec3d &ly, const Vec3d &lz, const Vec4d &q) {
    Vec3d p(Vec3d(q.x, q.y, q.z) - o);
    return {dot(lx, p), dot(ly, p), dot(lz, p), q.w};
}

struct CurveIntersection {
    std::uint32_t curveP0;
    double t;
    Point2d uv;
    double w;
};

struct StackNode {
    Vec4d p0, p1;
    double tMin, tMax;
    int depth;

    void set(double tMin_, double tMax_, Vec4d p0_, Vec4d p1_, int depth_) {
        p0 = p0_;
        p1 = p1_;
        tMin = tMin_;
        tMax = tMax_;
        depth = depth_;
    }
};

template<typename T>
static inline void precomputeBSplineCoefficients(T &p0, T &p1, T &p2) {
    T q0 = (p0 * 0.5 - p1 + p2 * 0.5);
    T q1 = (p1 - p0);
    T q2 = (p0 + p1) * 0.5;
    p0 = q0;
    p1 = q1;
    p2 = q2;
}

static inline void intersectHalfCylinder(StackNode node, double tMin,
                                         double &tMax, CurveIntersection &isect) {

    Vec2d v = Vec2d(node.p1.x - node.p0.x, node.p1.y - node.p0.y);
    double lengthSq = v.length2();
    double invLengthSq = 1 / lengthSq;
    double invLength = std::sqrt(invLengthSq);
    double segmentT = -(dot(Vec2d(node.p0.x, node.p0.y), v)) * invLengthSq;
    double signedUnnormalized = node.p0.x * v.y - node.p0.y * v.x;
    double distance = std::fabs(signedUnnormalized) * invLength;

    double width = node.p0.w * (1 - segmentT) + node.p1.w * segmentT;
    if (distance > width)
        return;

    double depth = node.p0.z * (1 - segmentT) + node.p1.z * segmentT;
    double dz = node.p1.z - node.p0.z;
    double ySq = sqr(width) - sqr(distance);
    double lSq = ySq * (1 + dz * dz * invLengthSq);
    double deltaT = std::sqrt(std::max(lSq, 0.0));
    double t0 = depth - deltaT;

    Vec3d v3(node.p0.x - node.p1.x, node.p0.y - node.p1.y, node.p0.z - node.p1.z);
    lengthSq = v3.length2();
    segmentT = dot((Vec3d(node.p0.x, node.p0.y, node.p0.z - t0)), v3) / lengthSq;
    if (segmentT < 0 || t0 >= tMax || t0 <= tMin) {
        return;
    }

    double newT = segmentT * (node.tMax - node.tMin) + node.tMin;

    if (newT >= 0 && newT <= 1) {
        isect.uv = Point2d(newT, 0.5 + 0.5 * distance / width);
        isect.t = t0;
        isect.w = width;
        tMax = t0;
    }
}

bool pointOnSpline(Vec4d q0, Vec4d q1, Vec4d q2,
                   double tMin, double tMax, CurveIntersection *isect) {
    const int MaxDepth = 5;

    StackNode stackBuf[MaxDepth];
    StackNode *stack = &stackBuf[0];

    precomputeBSplineCoefficients(q0, q1, q2);

    Vec4d tFlat = -q1 * 0.5 / q0;
    Vec4d xyFlat = (q0 * tFlat * tFlat + q1 * tFlat + q2);
    double xFlat = xyFlat.x, yFlat = xyFlat.y;

    StackNode cur{
        q2,
        q0 + q1 + q2,
        0, 1, 0};
    double closestDepth = tMax;

    while (true) {
        Vec2d pMin = Vec2d(std::min(cur.p0.x, cur.p1.x), std::min(cur.p0.y, cur.p1.y));
        Vec2d pMax = Vec2d(std::max(cur.p0.x, cur.p1.x), std::max(cur.p0.y, cur.p1.y));
        if (tFlat.x > cur.tMin && tFlat.x < cur.tMax) {
            pMin.x = std::min(pMin.x, xFlat);
            pMax.x = std::max(pMax.x, xFlat);
        }
        if (tFlat.y > cur.tMin && tFlat.y < cur.tMax) {
            pMin.y = std::min(pMin.y, yFlat);
            pMax.y = std::max(pMax.y, yFlat);
        }

        double maxWidth = std::max(cur.p0.w, cur.p1.w);
        if (pMin.x <= maxWidth && pMin.y <= maxWidth && pMax.x >= -maxWidth && pMax.y >= -maxWidth) {
            if (cur.depth >= MaxDepth) {
                intersectHalfCylinder(cur, tMin, closestDepth, *isect);
            } else {
                double splitT = (cur.tMin + cur.tMax) * 0.5;
                Vec4d qSplit = q0 * (splitT * splitT) + q1 * splitT + q2;

                if (cur.p0.z < qSplit.z) {
                    (*stack++).set(splitT, cur.tMax, qSplit, cur.p1, cur.depth + 1);
                    cur.set(cur.tMin, splitT, cur.p0, qSplit, cur.depth + 1);
                } else {
                    (*stack++).set(cur.tMin, splitT, cur.p0, qSplit, cur.depth + 1);
                    cur.set(splitT, cur.tMax, qSplit, cur.p1, cur.depth + 1);
                }
                continue;
            }
        }
        do {
            if (stack == stackBuf)
                return closestDepth < tMax;
            cur = *--stack;
        } while (std::min(cur.p0.z - cur.p0.w, cur.p1.z - cur.p1.w) > closestDepth);
    }

    return false;
}

class CurveSegment : public Entity {
public:
    BoundingBox3f WorldBound() const override {
        return box;
    }
    CurveSegment(std::vector<Vec4d> *_nodeData, int id) : _nodeData(_nodeData), id(id) {
        box = curveBox(_nodeData->operator[](id - 2), _nodeData->operator[](id - 1), _nodeData->operator[](id));
    }
    std::optional<Intersection> intersect(const Ray &r) const override {
        CurveIntersection curveIts{};
        bool didIntersect = false;
        Vec3d o(r.origin.x, r.origin.y, r.origin.z);
        Vec3d lz(r.direction);
        double d = std::sqrt(lz.x * lz.x + lz.z * lz.z);
        Vec3d lx, ly;
        if (d == 0) {
            lx = Vec3d(1, 0, 0);
            ly = Vec3d(0, 0, -lz.y);
        } else {
            lx = Vec3d(lz.z / d, 0, -lz.x / d);
            ly = Vec3d(lx.z * lz.y, d, -lz.y * lx.x);
        }

        Vec4d q0(project(o, lx, ly, lz, (*_nodeData)[id - 2]));
        Vec4d q1(project(o, lx, ly, lz, (*_nodeData)[id - 1]));
        Vec4d q2(project(o, lx, ly, lz, (*_nodeData)[id - 0]));
        if (pointOnSpline(q0, q1, q2, r.timeMin, r.timeMax, &curveIts)) {
            curveIts.curveP0 = id - 2;
            didIntersect = true;
        }

        if (!didIntersect) {
            return std::nullopt;
        }
        auto p0 = curveIts.curveP0;

        Intersection its{};

        auto tangentWithW = (quadraticDeriv((*_nodeData)[p0], (*_nodeData)[p0 + 1], (*_nodeData)[p0 + 2], curveIts.uv.x));
        Vec3d tangent = normalize(Vec3d(tangentWithW.x, tangentWithW.y, tangentWithW.z));

        its.geometryNormal = normalize((-r.direction - tangent * dot(tangent, -r.direction)));
        its.t = curveIts.t;
        its.object = this;
        its.uv = curveIts.uv;
        its.position = r.at(its.t);
        return {its};
    }
    Intersection sample(const Point2d &positionSample) const override {
        return {};
    }

    double area() const override {
        return 0;
    }

private:
    std::vector<Vec4d> *_nodeData;
    int id;
    BoundingBox3f box;
};

Curve::Curve(const Json &json) : Entity(json) {

    CurveIO::LoadCurve(json["file"], &_curveEnds, &_nodeData, &_nodeColor, &_nodeNormals);
    _curveCount = _curveEnds.size();

    _overrideThickness = containsAndGet(json, "curve_thickness", _curveThickness);
    bool tapper = getOptional(json, "curve_taper", false);
    if (_overrideThickness || tapper) {
        for (int i = 0; i < _curveCount; ++i) {
            int start = i ? _curveEnds[i - 1] : 0;
            for (int t = start; t < _curveEnds[i]; ++t) {
                double thickness = _overrideThickness ? _curveThickness : _nodeData[t].w;
                if (tapper)
                    thickness *= 1 - (t - start - 0.5) / (_curveEnds[i] - start - 1);
                _nodeData[t].w = thickness;
            }
        }
    }
    double widthScale = getOptional(json, "width_scale", 1.0);
    double transformMatrix[] = {0, 0, 0.1, 0, 0.13, 0, 0, 0, 0, 0.1, 0, 0, 0, 9.4, 0, 1};
    matrix = std::make_shared<TransformMatrix3D>(transformMatrix);

    for (int i = 0; i < _nodeData.size(); i++) {
        Point3d newP = matrix->operator*(Point3d(_nodeData[i].x, _nodeData[i].y, _nodeData[i].z));
        _nodeData[i].x = newP.x;
        _nodeData[i].y = newP.y;
        _nodeData[i].z = newP.z;
        _nodeData[i].w *= widthScale;
    }

    curveSegments.reserve(_nodeData.size() - 2 * _curveCount);

    for (std::uint32_t i = 0; i < _curveCount; i++) {
        std::uint32_t start = 0;
        if (i > 0) {
            start = _curveEnds[i - 1];
        }
        for (std::uint32_t t = start + 2; t < _curveEnds[i]; ++t) {
            curveSegments.emplace_back(std::make_shared<CurveSegment>(&_nodeData, t));
        }
    }

    for (const auto &curveSegment : curveSegments) {
        bb = BoundingBoxUnion(bb, curveSegment->WorldBound());
    }
    curveScene = std::make_unique<EmbreeAccel>(curveSegments);
}

std::optional<Intersection> Curve::intersect(const Ray &r) const {
    auto its = this->curveScene->Intersect(r);
    if (!its)
        return std::nullopt;
    its->material = material;
    its->object = this;
    return its;
}

double Curve::area() const {
    return 0;
    // todo
    return 1.0;
}

Intersection Curve::sample(const Point2d &positionSample) const {
    throw("Not Implement Yet");
}

BoundingBox3f Curve::WorldBound() const {
    return bb;
}

void Curve::apply() {
}
