#include "Thinlens.h"
#include "CoreLayer/Math/Warp.h"

ThinlensCamera::ThinlensCamera(const Json &json) : PerspectiveCamera(json) {

    apertureRadius = getOptional(json, "aperture_radius", 0.1);
    focusDistance = getOptional(json, "focus_distance", 10);
}

Ray ThinlensCamera::generateRay(const Point2i &filmResolution, const Point2i &pixelPosition, const CameraSample &sample) const {
    double x = (double)(pixelPosition.x + sample.xy.x) / filmResolution.x;
    double y = (double)(pixelPosition.y + sample.xy.y) / filmResolution.y;

    Point3d pointOnFilm = sampleToFilm * Point3d(x, y, 0);
    Point3d pointOnFocalPlane = pointOnFilm * (focusDistance / pointOnFilm.z);

    Point2d pointOnDisk = apertureRadius * SquareToUniformDiskConcentric(sample.lens);
    Point3d pointOnAperture = {pointOnDisk.x, pointOnDisk.y, 0};

    Vec3d dir = normalize(pointOnFocalPlane - pointOnAperture);

    return {cameraToWorld * pointOnAperture, cameraToWorld * dir};
}
