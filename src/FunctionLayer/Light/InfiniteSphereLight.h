/**
 * @file InfiniteSphereLight.h
 * @author JunPing Yuan
 * @brief InfiniteSphereLight
 * @version 0.1
 * @date 2022-9-27
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#pragma once

#include "Light.h"
#include "FunctionLayer/Texture/ImageTexture.h"
#include "FunctionLayer/Distribution/Distribution.h"
#include "CoreLayer/Geometry/Transform3d.h"


class InfiniteSphereLight : public  Light,Transform3D{
public:
    InfiniteSphereLight(const Json & json);

    LightSampleResult evalEnvironment(const Ray & ray) override;

    LightSampleResult eval(const Ray & ray, const Intersection & its, const Vec3d & d) override;

    LightSampleResult sampleEmit(const Point2d & positionSample, const Point2d & directionSample, float time) override;

    LightSampleResult sampleDirect(const Intersection & its, const Point2d & sample, float time) override;

    LightSampleResult sampleDirect(const MediumSampleRecord & mRec, Point2d sample, double time) override;
protected:
    double  directPdf(Vec3d dir);

    std::unique_ptr <Distribution2D> distribution;
    std::shared_ptr <ImageTexture<Spectrum,RGB3>> emission;
    TransformMatrix3D _toLocal;
    Vec3d UvToDirection(const Point2d &uv, double &sinTheta);
    Point2d DirectionToUv(const Vec3d &direction);
    Point2d DirectionToUv(const Vec3d &direction, double &sinTheta);
};