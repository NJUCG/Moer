#include "InfiniteSphereCapLight.h"
#include "FastMath.h"

static inline Vec3d uniformSphericalCap(const Point2d & xi, double cosThetaMax) {
    double phi = xi.x * 2 * M_PI;
    double z = xi.y * ( 1.0f - cosThetaMax ) + cosThetaMax;
    double r = fm::sqrt(std::max(1.0 - z * z, 0.0));
    return Vec3d(
            fm::cos(phi) * r,
            fm::sin(phi) * r,
            z
    );
}

static inline double uniformSphericalCapPdf(double cosThetaMax) {
    return 0.5 / M_PI / ( 1.0f - cosThetaMax );
}

InfiniteSphereCapLight::InfiniteSphereCapLight(const Json & json) : Light(ELightType::INFINITE), Transform3D(
        getOptional(json, "transform", Json())) {
    _capAngle = Angle(getOptional(json, "cap_angle", 5), Angle::EAngleType::ANGLE_DEG).getRad();
    _cosCapAngle = cos(_capAngle);
     if(json.contains("cap_dir"))
         _capDir = json["cap_dir"];
     else
         _capDir = matrix->operator *(Vec3d(0, 1, 0));
    _capFrame = Frame(_capDir);
    _emission = getOptional(json, "emission", Spectrum(1));
    if ( json.contains("power") ) {
        Spectrum power = json["power"];
        _emission = power * INV_TWOPI / ( 1.0f - _cosCapAngle );
    }
}

LightSampleResult InfiniteSphereCapLight::evalEnvironment(const Ray & ray) {

    //todo support environment
    LightSampleResult ans;
    if ( dot(ray.direction, _capDir) < _cosCapAngle )
        return ans;
    ans.s = _emission;
    ans.src = ray.origin;
    ans.pdfDirect = 0.0;
    ans.pdfEmitPos = 0.0;
    ans.pdfEmitDir = directPdf(ray.direction);
    ans.isDeltaPos = false;
    ans.isDeltaDir = false;
    return ans;
}

LightSampleResult InfiniteSphereCapLight::eval(const Ray & ray, const Intersection & its, const Vec3d & d) {
    return LightSampleResult();
}

LightSampleResult
InfiniteSphereCapLight::sampleEmit(const Point2d & positionSample, const Point2d & directionSample, float time) {
    return LightSampleResult();
}

LightSampleResult InfiniteSphereCapLight::sampleDirect(const Intersection & its, const Point2d & sample, float time) {
    LightSampleResult result;
    result.s = _emission;
    result.wi = _capFrame.toWorld(uniformSphericalCap(sample, _cosCapAngle));
    result.src = its.position;
    _worldRadius = 200; //todo load radius from scene
    result.dst = result.src + 2 * _worldRadius * result.wi;
    result.pdfDirect = uniformSphericalCapPdf(_cosCapAngle);
    result.isDeltaDir = false;
    result.isDeltaPos = false;
    return result;
}

LightSampleResult InfiniteSphereCapLight::sampleDirect(const MediumSampleRecord & mRec, Point2d sample, double time) {
    return LightSampleResult();
}

double InfiniteSphereCapLight::directPdf(Vec3d dir) {
    if ( dot(dir, _capDir) < _cosCapAngle )
        return 0;
    return uniformSphericalCapPdf(_cosCapAngle);
}
