#pragma once

#include "Perspective.h"

/**
 * @brief Pinhole camera
 * @ingroup Camera
 */
class PinholeCamera : public PerspectiveCamera {
public:
    PinholeCamera() = default;
    PinholeCamera(
        const Point3d &lookFrom, 
        const Point3d &lookAt, 
        const Vec3d &up,
        double xFov,
        double aspectRatio,
        double distToFilm
    ) : PerspectiveCamera(lookFrom, lookAt, up, xFov, aspectRatio, distToFilm) { }

    /**
     * @brief 
     * ! Notice, NDC is the sample in unit square
     * @param NDC 
     * @return Ray 
     */

    virtual Ray generateRay(const Point2i &filmResolution, 
                            const Point2i &pixelPosition, 
                            const CameraSample &sample) const override;
};