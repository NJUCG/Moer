#include "Quad.h"
#include "FunctionLayer/Intersection.h"

Quad::Quad(const Json & json) : Entity(json) {
    _base = getOptional(json,"base",Point3d(0,0,0));
    _edge0 = getOptional(json,"edge0",Vec3d(1,0,0));
    _edge1 = getOptional(json,"edge1",Vec3d(0,0,1));
    apply();
}




std::optional <Intersection> Quad::intersect(const Ray & r) const {
    Vec3d  n = normalize(cross(_edge1,_edge0));;
    double dotW = dot(r.direction,n);
    if(std::abs(dotW)<EPSILON){
        return std::nullopt;
    }
    double t = dot(n,(_base - r.origin))/dotW;

    if (t < r.timeMin || t > r.timeMax)
        return std::nullopt;

    Point3d q = r.at(t);
    Vec3d v = q - _base;
    double l0 = dot(v,_edge0) * _invSq.x;
    double l1 = dot(v,_edge1) * _invSq.y;


    if (l0 < 0.0f || l0 > 1.0f || l1 < 0.0f || l1 > 1.0f)
        return std::nullopt;


    Intersection ans;
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

std::shared_ptr < Light > Quad::getLight( ) const {
    return  lightPtr;
}

void Quad::setLight(std::shared_ptr < Light > light) {
    lightPtr = light;
}

double Quad::area( ) const {
    return _area;
}

Intersection Quad::sample(const Point2d & positionSample) const {
    Intersection its;
    its.uv = positionSample;
    its.position = _base + _edge0 * positionSample.x + _edge1 * positionSample.y;
    its.geometryNormal = normalize(cross(_edge1,_edge0));
    return its;
}

BoundingBox3f Quad::WorldBound( ) const {
    BoundingBox3f bb;
    bb = BoundingBoxPointUnion(bb,_base);
    bb = BoundingBoxPointUnion(bb,_base+_edge0);
    bb = BoundingBoxPointUnion(bb,_base+_edge1);
    bb = BoundingBoxPointUnion(bb,_base+_edge0+_edge1);

    return bb;

}

void Quad::apply( ) {

//    _base = matrix->operator *(_base);
//    _edge0 = matrix->operator *(_edge0);
//    _edge1 = matrix->operator *(_edge1);
//    _base -= _edge0*0.5;
//    _base -= _edge1*0.5;

    _area  =  cross(_edge0,_edge1).length();
    _invArea = 1/_area;
    _invSq = Vec2d (1/ _edge0.length2(),1/_edge1.length2());
}

void rtcQuadBoundsFunc(const RTCBoundsFunctionArguments *args) 
{
    Quad *quad = (Quad *)args->geometryUserPtr;
    RTCBounds *bounds = args->bounds_o;

    BoundingBox3f box = quad->WorldBound();
    
    bounds->lower_x = box.pMin.x;
    bounds->lower_y = box.pMin.y;
    bounds->lower_z = box.pMin.z;

    bounds->upper_x = box.pMax.x;
    bounds->upper_y = box.pMax.y;
    bounds->upper_z = box.pMax.z;

}

void rtcQuadIntersectFunc(const RTCIntersectFunctionNArguments *args)
{
    int *valid = args->valid;
    if (!valid[0])
        return;

    Quad *quad = (Quad *)args->geometryUserPtr;
    auto base = quad->_base;
    auto edge0 = quad->_edge0,
         edge1 = quad->_edge1;

    RTCRayHitN *rayhit = args->rayhit;

    RTCRayN *ray = RTCRayHitN_RayN(rayhit, 1);
    RTCHitN *hit = RTCRayHitN_HitN(rayhit, 1);

    auto ori_x = RTCRayN_org_x(ray, 1, 0),
         ori_y = RTCRayN_org_y(ray, 1, 0),
         ori_z = RTCRayN_org_z(ray, 1, 0),
         dir_x = RTCRayN_dir_x(ray, 1, 0),
         dir_y = RTCRayN_dir_y(ray, 1, 0),
         dir_z = RTCRayN_dir_z(ray, 1, 0);

    auto tnear = RTCRayN_tnear(ray, 1, 0),
         tfar = RTCRayN_tfar(ray, 1, 0);

    Point3d ori{ori_x, ori_y, ori_z};
    Vec3d dir{dir_x, dir_y, dir_z};

    Vec3d n = normalize(cross(edge1, edge0));
    double dirDotN = dot(dir, n);
    if (std::abs(dirDotN) < 1e-9) {
        return;
    }

    double t = dot(n, base - ori) / dirDotN;
    if (t < tnear || t > tfar) 
        return;

    Point3d hitpoint = ori + dir * t;

    double u = dot(hitpoint - base, edge0) / edge0.length2(),
           v = dot(hitpoint - base, edge1) / edge1.length2();
    
    if ((u < 0 || u > 1) || (v < 0 || v > 1))
        return;

    RTCHit result;
    result.geomID = args->geomID;
    result.u = u;
    result.v = v;
    
    RTCFilterFunctionNArguments fargs;
    int imask = -1;
    fargs.valid = &imask;
    fargs.geometryUserPtr = args->geometryUserPtr;
    fargs.context = args->context;
    fargs.ray = (RTCRayN *)args->rayhit;
    fargs.hit = (RTCHitN *) &result;
    fargs.N = 1;

    rtcFilterIntersection(args, &fargs);
    if (*fargs.valid == -1){
        *((RTCHit *)hit) = result;
        RTCRayN_tfar(ray, 1, 0) = t;
    }
    return;

}

//TODO
void rtcQuadOccludeFunc(const RTCOccludedFunctionNArguments *args) {
    return;
}
/*
RTCGeometry Quad::toEmbreeGeometry(RTCDevice device) const 
{
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryUserPrimitiveCount(geom, 1);
    rtcSetGeometryUserData(geom, (void *)this);
    rtcSetGeometryBoundsFunction(geom, rtcQuadBoundsFunc, nullptr);
    rtcSetGeometryIntersectFunction(geom,rtcQuadIntersectFunc);
    rtcSetGeometryOccludedFunction(geom, rtcQuadOccludeFunc);
    rtcCommitGeometry(geom);
    return geom;
}
*/
EntitySurfaceInfo Quad::getEntitySurfaceInfo(int primID, Point2d _uv) const
{
    Normal3d normal = normalize(cross(_edge1, _edge0));
    auto [u, v] = _uv;
    return {_base + _edge0 * u + _edge1 * v, normal, 
            Normal3d(), Normal3d(),
            _uv};
}





