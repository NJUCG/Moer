#include "Pinhole.h"

Ray PinholeCamera::generateRay(Point2d NDC) const {
    Point3d pointOnFilm = sampleToFilm * Point3d {NDC.x, NDC.y, 0},
            origin = Point3d {0, 0, 0};
    Vec3d dir =  normalize(pointOnFilm - origin);
    return Ray(
        cameraToWorld * origin,
        cameraToWorld * dir        
    );
}

Ray PinholeCamera::generateRay(Point2d NDC, Point2d sample) const {
    // no need to use the sample
    return generateRay(NDC);
}