#include "InfiniteSphereLight.h"
#include "ResourceLayer/File/FileUtils.h"

Vec3d UvToDirection(const Point2d & uv, double & sinTheta) {
    double phi = ( uv.x - 0.5 ) * 2 * M_PI;
    double theta = uv.y * M_PI;
    sinTheta = std::sin(theta);
    return Vec3d(
            std::cos(phi) * sinTheta,
            -std::cos(theta),
            std::sin(phi) * sinTheta);
}

Point2d DirectionToUv(const Vec3d & direction) {
    return Point2d(std::atan2(direction.z, direction.x) * 0.5 * INV_PI + 0.5, std::acos(- direction.y) * INV_PI);
}

Point2d DirectionToUv(const Vec3d & direction, double & sinTheta) {
    sinTheta = sqrt(1 - direction.y * direction.y);
    return Point2d(std::atan2(direction.z, direction.x) * 0.5 * INV_PI + 0.5, std::acos(- direction.y) * INV_PI);
}


LightSampleResult InfiniteSphereLight::evalEnvironment(const Ray & ray) {


    //todo support environment
    LightSampleResult ans;
    TextureCoord2D textureCoord2D(DirectionToUv(ray.direction));
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
    return LightSampleResult();
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
    ans.pdfDirect = pdf  / ( 2 * M_PI * M_PI * sinTheta );
    ans.pdfDirect = directPdf(dir);
  //  ans.wi = Vec3d(uv.x,uv.y,0);
    ans.wi = dir;
    ans.isDeltaPos = false;
    ans.isDeltaDir = false;

    return ans;
}

LightSampleResult InfiniteSphereLight::sampleDirect(const MediumSampleRecord & mRec, Point2d sample, double time) {
    throw ( "Not implemented yet" );
    return LightSampleResult();
}

InfiniteSphereLight::InfiniteSphereLight(const Json & json) : Light(ELightType::INFINITE) {
    if ( json.contains("emission") ) {
        std::string workingDir = FileUtils::WorkingDir;
        std::string emissionTexturePath = FileUtils::WorkingDir + json.at("emission").get < std::string >();
        emission = std::make_shared < ImageTexture < Spectrum, RGB3>>(
                FileUtils::WorkingDir + json.at("emission").get < std::string >());
        std::vector < double > weights = emission->sphericalWeighs();
        //todo sphere weights
        distribution = std::make_unique < Distribution2D >(weights.data(), emission->getWidth(), emission->getHeight());
    } else {
        //todo report error
    }
}

double InfiniteSphereLight::directPdf(Vec3d dir) {
    double sinTheta;
    Point2d uv = DirectionToUv(dir, sinTheta);
    return INV_PI * INV_TWOPI * distribution->Pdf(uv)  / sinTheta;
}
