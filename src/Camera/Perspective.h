/**
 * @file Perspective.h
 * @author Chenxi Zhou
 * @brief Abstract base class for all perspective camera
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include "../Core/FunctionLayer/Camera.h"
#include "../Core/FunctionLayer/Transform3d.h"

class PerspectiveCamera : public Camera {
protected:
public:
    // TODO replace martrix with transform3d
    //! cameraToWorld : Turn a vector in camera coordinate to world coordinate
    //! sampleToFilm :  Turn a point [x, y, 0] to the corresponding point on film
    //
    //     [0, 0]---------------> x
    //          |      |
    //          |------+ [x, y]
    //          |
    //          y
    Matrix4x4 cameraToWorld, sampleToFilm;
public:
    PerspectiveCamera() = default;
    
    /**
     * @brief Construct a new Perspective Camera object
     * 
     * @param lookFrom      pinhole location
     * @param lookAt        look at position
     * @param up            up vector
     * @param xFov          fov on x (left-right) degree
     * @param aspectRatio   film Width / film Height
     * @param distToFilm    distance from pinhole to film  
     */
    PerspectiveCamera(
        const Point3d &lookFrom, 
        const Point3d &lookAt, 
        const Vec3d &up,
        double xFov,
        double aspectRatio,
        double distToFilm
    )
    {
        cameraToWorld = 
            Matrix4x4::lookAt(lookFrom, lookAt - lookFrom, up).inverse();
        // ! near is the distToFilm and far set to MAX_FLOAT 
        // ! if far set to MAX_DOUBLE, it will crash when computing the matrix 
        Matrix4x4 filmToSample = Matrix4x4::perspective(
            Angle(xFov, Angle::AngleType::ANGLE_DEG), 
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

    virtual Ray generateRay(Point2d NDC) const = 0;

    virtual Ray generateRay(Point2d NDC, Point2d sample) const = 0;
};