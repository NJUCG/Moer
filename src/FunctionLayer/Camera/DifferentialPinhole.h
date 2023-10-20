#pragma once

#include "Perspective.h"
#include "CoreLayer/Adapter/JsonUtil.h"

// pinhole with differential
class DifferentialPinholeCamera : public PerspectiveCamera {
public:
    DifferentialPinholeCamera() = default;
    DifferentialPinholeCamera(
        const Point3d &lookFrom, 
        const Point3d &lookAt, 
        const Vec3d &up,
        double xFov,
        double aspectRatio,
        double distToFilm
    ) : PerspectiveCamera(lookFrom, lookAt, up, xFov, aspectRatio, distToFilm) { }

    DifferentialPinholeCamera(const Json & json);

    virtual Ray generateRay(const Point2i &filmResolution, 
                            const Point2i &pixelPosition, 
                            const CameraSample &sample) const override;
};