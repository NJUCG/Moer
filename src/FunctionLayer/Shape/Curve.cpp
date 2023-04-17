#include "Curve.h"
#include "ResourceLayer/File/CurveIO.h"
#include "FunctionLayer/Intersection.h"
#include "CoreLayer/Geometry/BoundingBox.h"



template < typename T >
inline T quadraticDeriv(T p0, T p1, T p2, T p3, float t) {

    const double t0 = 1.0f - t, t1 = t;
    const double B0 = - ( t0 * t0 );
    const double B1 = - 2.0f * ( t0 * t1 ) + ( t0 * t0 );
    const double B2 = + 2.0f * ( t0 * t1 ) - ( t1 * t1 );
    const double B3 = + ( t1 * t1 );
    return 3.0 * ( B0 * p0 + B1 * p1 + B2 * p2 + B3 * p3 );
}

template < typename T >
inline T quadratic(T p0, T p1, T p2, float t) {
    return ( 0.5 * t * t - t + 0.5 ) * p0 + ( t - t * t + 0.5 ) * p1 + 0.5 * t * t * p2;

}

template < typename T >
inline T quadratic(T p0, T p1, T p2, T p3, float t) {
    return quadratic(lerp(p0, p1, t), lerp(p1, p2, t), lerp(p2, p3, t), t);
}

inline Vec2d quadraticMinMax(double p0, double p1, double p2) {
    double xMin = ( p0 + p1 ) * 0.5f;
    double xMax = ( p1 + p2 ) * 0.5f;
    if ( xMin > xMax )
        std::swap(xMin, xMax);

    double tFlat = ( p0 - p1 ) / ( p0 - 2.0f * p1 + p2 );
    if ( tFlat > 0.0f && tFlat < 1.0f ) {
        double xFlat = quadratic(p0, p1, p2, tFlat);
        xMin = std::min(xMin, xFlat);
        xMax = std::max(xMax, xFlat);
    }
    return Vec2d(xMin, xMax);
}



static BoundingBox3f curveBox(const double * q0, const double * q1, const double * q2) {
    Vec2d xMinMax(quadraticMinMax(q0[0], q1[0], q2[0]));
    Vec2d yMinMax(quadraticMinMax(q0[1], q1[1], q2[1]));
    Vec2d zMinMax(quadraticMinMax(q0[2], q1[2], q2[2]));
    double maxW = std::max(q0[3], std::max(q1[3], q2[3]));
    return BoundingBox3f(
            Point3d(xMinMax.x, yMinMax.x, zMinMax.x) - Vec3d(maxW),
            Point3d(xMinMax.y, yMinMax.y, zMinMax.y) + Vec3d(maxW)
    );
}

Curve::Curve(const Json & json) : Entity(json) {

    CurveIO::LoadCurve(json["file"], & _curveEnds, & _nodeData, & _nodeColor, & _nodeNormals);
    _curveCount = _curveEnds.size();

    _overrideThickness = containsAndGet(json, "curve_thickness", _curveThickness);
    bool tapper = getOptional(json, "curve_taper", false);
    if ( _overrideThickness || tapper ) {
        for ( int i = 0 ; i < _curveCount ; ++ i ) {
            int start = i ? _curveEnds[i - 1] : 0;
            for ( int t = start ; t < _curveEnds[i] ; ++ t ) {
                double thickness = _overrideThickness ? _curveThickness : _nodeData[t * 4 + 3];
                if ( tapper )
                    thickness *= 1.0f - ( t - start - 0.5f ) / ( _curveEnds[i] - start - 1 );
                _nodeData[t * 4 + 3] = thickness;
            }
        }
    }
    double widthScale = getOptional(json, "width_scale", 1.0);
    double transformMatrix[] =  {0,0,0.1,0,0.13,0,0,0,0,0.1,0,0,0,9.4,0,1};
    matrix = std::make_shared <TransformMatrix3D>(transformMatrix);

    for ( int i = 0 ; i < _nodeData.size()/4 ; i ++ ) {
        int begin = 4 * i;
        Point3d newP = matrix->operator *(Point3d(_nodeData[begin], _nodeData[begin + 1], _nodeData[begin + 2]));
        _nodeData[begin] = newP.x;
        _nodeData[begin + 1] = newP.y;
        _nodeData[begin + 2] = newP.z;
        _nodeData[begin + 3] *= widthScale;
    }

    int start = 0;
    for ( int i = 0 ; i < _curveCount ; i ++ ) {
        if ( i > 0 ) {
            start = _curveEnds[i - 1];
        }
        for ( int t = start + 3 ; t < _curveEnds[i] ; ++ t ) {
            _indices.push_back(t - 3);
        }
    }

    ///compute bb
    for ( size_t i = 8 ; i < _nodeData.size() ; i+=4 )
        bb = BoundingBoxUnion(bb,
                              curveBox(& _nodeData[i-8], & _nodeData[i-4], & _nodeData[i]));
}

RTCGeometry Curve::toEmbreeGeometry(RTCDevice device) const {
    auto geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_FLAT_BSPLINE_CURVE);
    float * vb =  (float *)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX, 0,
                                                     RTC_FORMAT_FLOAT4,
                                                     4 * sizeof(float), _nodeData.size()/4);
    for(int i =0 ;i<_nodeData.size();i++)
        vb[i] = _nodeData[i];
    unsigned * ib = (unsigned *) rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX, 0,
                                                         RTC_FORMAT_UINT,
                                                         sizeof(unsigned), _indices.size());
    memcpy(ib, _indices.data(), _indices.size());
    rtcCommitGeometry(geometry);
    return geometry;
}

std::optional < Intersection > Curve::getIntersectionFromRayHit(const UserRayHit1 & rayhit) const {
    Vec3d rayDir = {rayhit.ray.dir_x, rayhit.ray.dir_y, rayhit.ray.dir_z};
    Point3d rayOrigin {rayhit.ray.org_x,rayhit.ray.org_y,rayhit.ray.org_z};

    Intersection its;
    its.t = rayhit.ray.tfar;
    its.object = this;
    its.material = this->material;
    its.position = rayOrigin + rayDir * its.t;
    auto p0 = _indices[rayhit.hit.primID];
    float t = rayhit.hit.u;
    auto q = quadraticDeriv(getPoint(p0), getPoint(p0 + 1), getPoint(p0 + 2), getPoint(p0 + 3), t);
    Vec3d tangent = normalize(Vec3d(q.x, q.y, q.z));
    its.geometryNormal = normalize(-rayDir - tangent * dot(tangent, -rayDir));
    its.geometryTangent = tangent;

    its.uv = {rayhit.hit.u, rayhit.hit.v};
    return {its};
}


std::optional < Intersection > Curve::intersect(const Ray & r) const {
    ///use embree intersect
    return std::optional<Intersection>{};
}

double Curve::area( ) const {
    //todo
    return 1.0;
}

Intersection Curve::sample(const Point2d & positionSample) const {
    throw ( "Not Implement Yet" );
}

BoundingBox3f Curve::WorldBound( ) const {
    return bb;
}

void Curve::apply( ) {
    /// Has  done in constructor;
}

Point3d Curve::getPoint(int i) const {
    return Point3d(_nodeData[4 * i], _nodeData[4 * i + 1], _nodeData[4 * i + 2]);
}
