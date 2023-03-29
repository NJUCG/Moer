/**
 * @file Perspective.h
 * @author Chenxi Zhou
 * @brief Abstract base class for all perspective camera
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 * 
 */
#pragma once

#include "Camera.h"
#include "CoreLayer/Geometry/Matrix.h"
#include "CoreLayer/Adapter/JsonUtil.h"
#include "FastMath.h"
/**
 * @brief Base class for all perspective camera
 * @ingroup Camera
 */
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

    PerspectiveCamera(const Json & json){
        const Json & transformJson = json["transform"];
        Point3d lookFrom = getOptional(transformJson,"position",Point3d(0,0,0));
        Point3d lookAt = getOptional(transformJson,"look_at",Point3d(0,1,0));
        Vec3d up = getOptional(transformJson,"up",Vec3d(0,0,1));

        double xFov = getOptional(json,"fov",45);
        Vec2i resolution = getOptional(json,"resolution",Vec2i(512,512));
        double  aspectRatio = double(resolution.x) / resolution.y;
        double distToFilm= 1.0f / fm::tan(xFov * M_PI / 360);

        cameraToWorld =
                Matrix4x4::lookAt(lookFrom, lookAt - lookFrom, up).inverse();
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
};