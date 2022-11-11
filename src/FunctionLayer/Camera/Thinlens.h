#pragma once

#include "Perspective.h"

/**
 * @brief Thinlens camera
 * @ingroup Camera
 */
class ThinlensCamera : public PerspectiveCamera {
protected:
    double apertureRadius, focalLen, focalDistance;
public:
    ThinlensCamera() = default;
    ThinlensCamera(
        const Point3d &lookFrom, 
        const Point3d &lookAt, 
        const Vec3d &up,
        double xFov,
        double aspectRatio,
        double _focaDistance,
        double _apertureRadius = 0.1,
        double _focalLen = 0.05
    ):apertureRadius(_apertureRadius), focalLen(_focalLen), focalDistance(_focaDistance) {
        double distToFilm = 
            (_focalLen * _focaDistance) / (_focaDistance - _focalLen);
        
        cameraToWorld = 
            Matrix4x4::lookAt(lookFrom, lookAt - lookFrom, up).inverse();
        // ! near is the distToFilm and far set to MAX_FLOAT 
        // ! if far set to MAX_DOUBLE, it will crash when computing the matrix 
        Matrix4x4 filmToSample = Matrix4x4::perspective(
            Angle(xFov, Angle::EAngleType::ANGLE_DEG), 
            aspectRatio, 
            distToFilm, 
            std::numeric_limits<float>::max()
        );
        sampleToFilm =
            Matrix4x4::scale(0.5, -0.5, 1.0)
            * Matrix4x4::translate(1, -1, 0) 
            * filmToSample;
        sampleToFilm = sampleToFilm.inverse();

    }


    virtual Ray generateRay(const Point2i &filmResolution ,
                            const Point2i &pixelPosition, 
                            const CameraSample &sample) const override;
};