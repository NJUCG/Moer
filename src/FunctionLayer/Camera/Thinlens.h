#pragma once

#include "Perspective.h"

/**
 * @brief Thinlens camera
 * @ingroup Camera
 */
class ThinlensCamera : public PerspectiveCamera {
public:

    explicit ThinlensCamera(const Json &json);

    [[nodiscard]]
    Ray generateRay(const Point2i &filmResolution, const Point2i &pixelPosition, const CameraSample &sample) const override;

protected:

    double apertureRadius;
    double focusDistance;

};
