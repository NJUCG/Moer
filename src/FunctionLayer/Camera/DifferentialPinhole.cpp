#include"DifferentialPinhole.h"

Ray DifferentialPinholeCamera::generateRay(const Point2i &filmResolution, const Point2i &pixelPosition, const CameraSample &sample) const {
    double x = (double)(pixelPosition.x + sample.xy.x) / filmResolution.x,
           y = (double)(pixelPosition.y + sample.xy.y) / filmResolution.y;
    
    Point3d pointOnFilm = sampleToFilm * Point3d {x, y, 0},
            origin = Point3d {0, 0, 0};
    
    Vec3d dir = cameraToWorld * normalize(pointOnFilm - origin);
    Vec3d dirX = cameraToWorld * normalize((sampleToFilm * Point3d {x + 1.f, y, 0}) - origin);
    Vec3d dirY = cameraToWorld * normalize((sampleToFilm * Point3d {x, y + 1.f, 0}) - origin);
    
    origin = cameraToWorld * origin;

    Ray ret = Ray(origin, dir);

    ret.hasDifferential = true;
    ret.direction_x = dirX;
    ret.direction_y = dirY;
    ret.origin_x = ret.origin_y = origin;

    return ret;
}

DifferentialPinholeCamera::DifferentialPinholeCamera(const Json & json) : PerspectiveCamera(json) {
}