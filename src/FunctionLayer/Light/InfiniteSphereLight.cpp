#include "InfiniteSphereLight.h"
#include "ResourceLayer/File/FileUtils.h"
#include "FastMath.h"

Vec3d InfiniteSphereLight::UvToDirection(const Point2d & uv, double & sinTheta) {
    double phi = ( uv.x - 0.5 ) * 2 * M_PI;
    double theta = uv.y * M_PI;
    sinTheta = fm::sin(theta);
    return  matrix->operator*(Vec3d(
            fm::cos(phi) * sinTheta,
            -fm::cos(theta),
            fm::sin(phi) * sinTheta));
}

Point2d InfiniteSphereLight::DirectionToUv(const Vec3d & direction) {
    auto localDir = _toLocal.operator*(direction);
    return Point2d(fm::atan2(localDir.z, localDir.x) * 0.5 * INV_PI + 0.5, fm::acos(-localDir.y) * INV_PI);
}

Point2d InfiniteSphereLight::DirectionToUv(const Vec3d & direction, double & sinTheta) {
    auto localDir = _toLocal.operator*(direction);
    sinTheta = sqrt(1 - localDir.y * localDir.y);
    return Point2d(fm::atan2(localDir.z, localDir.x) * 0.5 * INV_PI + 0.5, fm::acos(- localDir.y) * INV_PI);
}


LightSampleResult InfiniteSphereLight::evalEnvironment(const Ray & ray) {


    //todo support environment
    LightSampleResult ans;
    TextureCoord2D textureCoord2D(DirectionToUv( ray.direction));
    ans.s = emission->eval(textureCoord2D);
    ans.src = ray.origin;
    ans.pdfDirect = 0.0;
    ans.pdfEmitPos = 0.0;
    ans.pdfEmitDir = directPdf(ray.direction);
    ans.isDeltaPos = false;
    ans.isDeltaDir = false;
    return ans;
}

LightSampleResult InfiniteSphereLight::eval(const Ray & ray, const Intersection & its, const Vec3d & d) {
    throw ( "Infinite sphere should not be intersected,so this Function should  never not be called." );
}

LightSampleResult
InfiniteSphereLight::sampleEmit(const Point2d & positionSample, const Point2d & directionSample, float time) {
    throw ( "Not implemented yet" );
}

LightSampleResult InfiniteSphereLight::sampleDirect(const Intersection & its, const Point2d & sample, float time) {
    LightSampleResult ans;
    //todo map from image
    double pdf;
    Point2d uv = distribution->SampleContinuous(sample, &pdf);
    if(pdf==0){
        ans.s = Spectrum(0);
        return ans;
    }
    TextureCoord2D textureCoord2D(uv);
    ans.s = emission->eval(textureCoord2D);
    double sinTheta;
    Vec3d dir = UvToDirection(uv, sinTheta);

    ans.src = its.position;
    double _worldRadius = 100; //todo load radius from scene
    ans.dst = ans.src + 2 * _worldRadius * dir;
    ans.uv = uv;
    ans.pdfDirect = pdf * emission->getWidth() * emission->getHeight() / ( 2 * M_PI * M_PI * sinTheta );
    ans.wi = dir;
    ans.isDeltaPos = false;
    ans.isDeltaDir = false;
    return ans;
}

LightSampleResult InfiniteSphereLight::sampleDirect(const MediumSampleRecord & mRec, Point2d sample, double time) {
    throw ( "Not implemented yet" );
}

InfiniteSphereLight::InfiniteSphereLight(const Json & json) : Light(ELightType::INFINITE), Transform3D(getOptional(json,"transform",Json())) {
    if ( json.contains("emission") ) {
        std::string workingDir = FileUtils::getWorkingDir();
        std::string emissionTexturePath = FileUtils::getWorkingDir() + json.at("emission").get < std::string >();
        emission = std::make_shared < ImageTexture < Spectrum, RGB3>>(
                FileUtils::getWorkingDir() + json.at("emission").get < std::string >());
        std::vector < double > weights = emission->sphericalWeighs();
        //todo sphere weights
        distribution = std::make_unique < Distribution2D >(weights.data(), emission->getWidth(), emission->getHeight());
    } else {
        //todo report error
    }
    _toLocal = matrix->getInverse();
}

double InfiniteSphereLight::directPdf(Vec3d dir) {
    double sinTheta;
    Point2d uv = DirectionToUv( dir, sinTheta);
    return INV_PI * INV_TWOPI * distribution->Pdf(uv)   / sinTheta;
}
