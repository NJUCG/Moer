#include "Pinhole.h"

Ray PinholeCamera::generateRay(const Point2i &filmResolution, const Point2i &pixelPosition, const CameraSample &sample) const {
    double x = (double)(pixelPosition.x + sample.xy.x) / filmResolution.x,
           y = (double)(pixelPosition.y + sample.xy.y) / filmResolution.y;
    
    Point3d pointOnFilm = sampleToFilm * Point3d {x, y, 0},
            origin = Point3d {0, 0, 0};
    
    Vec3d dir =  normalize(pointOnFilm - origin);
    
    return Ray(
        cameraToWorld * origin,
        cameraToWorld * dir        
    );
}

PinholeCamera::PinholeCamera(const Json & json) : PerspectiveCamera(json) {
}