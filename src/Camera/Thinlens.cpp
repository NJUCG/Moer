#include "Thinlens.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

Ray ThinlensCamera::generateRay(Point2d NDC) const {
    Point3d pointOnFilm = sampleToFilm * Point3d (NDC.x, NDC.y, 0),
            pointOnFocalPlane = pointOnFilm * (focalDistance / pointOnFilm.z);
    // TODO warp the sample transform, and using another sample
    float r = std::sqrt(NDC[0]),
          theta = NDC[1] * 2 * M_PI;
    Point3d pointOnApeture = 
        apertureRadius * Point3d(r * std::cos(theta), r * std::sin(theta), 0);       
    Vec3d dir = normalize(pointOnFocalPlane - pointOnApeture);
        return Ray(
        cameraToWorld * pointOnApeture,
        cameraToWorld * dir
    );
}

Ray ThinlensCamera::generateRay(Point2d NDC, Point2d sample) const {
    Point3d pointOnFilm = sampleToFilm * Point3d (NDC.x, NDC.y, 0),
            pointOnFocalPlane = pointOnFilm * (focalDistance / pointOnFilm.z);
    // TODO warp the sample transform, and using another sample
    float r = std::sqrt(sample[0]),
          theta = sample[1] * 2 * M_PI;
    Point3d pointOnApeture = 
        apertureRadius * Point3d(r * std::cos(theta), r * std::sin(theta), 0);       
    Vec3d dir = normalize(pointOnFocalPlane - pointOnApeture);
        return Ray(
        cameraToWorld * pointOnApeture,
        cameraToWorld * dir
    );
}