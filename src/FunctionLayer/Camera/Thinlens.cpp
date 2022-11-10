#include <cmath>
#include "Thinlens.h"
#include "CoreLayer/Math/Common.h"

#define _USE_MATH_DEFINES

Ray ThinlensCamera::generateRay(const Point2i &filmResolution, const Point2i &pixelPosition, const CameraSample &sample) const {
    double x = (double) (pixelPosition.x + sample.xy.x) / filmResolution.x,
           y = (double) (pixelPosition.y + sample.xy.y) / filmResolution.y;
    
    Point3d pointOnFilm = sampleToFilm * Point3d (x, y, 0),
            pointOnFocalPlane = pointOnFilm * (focalDistance / pointOnFilm.z);
    // TODO warp the sample transform
    float r = std::sqrt(sample.lens[0]),
          theta = sample.lens[1] * 2 * M_PI;
    Point3d pointOnApeture = 
        apertureRadius * Point3d(r * std::cos(theta), r * std::sin(theta), 0);       
    Vec3d dir = normalize(pointOnFocalPlane - pointOnApeture);
        return Ray(
        cameraToWorld * pointOnApeture,
        cameraToWorld * dir
    );
}