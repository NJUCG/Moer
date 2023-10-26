/**
 * @file InfiniteSphereCapLight.h
 * @author JunPing Yuan
 * @brief InfiniteSphereCapLight
 * @version 0.1
 * @date 2022-11-25
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

class InfiniteSphereCapLight : public  Light,Transform3D{
public:
    InfiniteSphereCapLight(const Json & json);

    LightSampleResult evalEnvironment(const Ray & ray) override;

    LightSampleResult eval(const Ray & ray, const Intersection & its, const Vec3d & d) override;

    LightSampleResult sampleEmit(const Point2d & positionSample, const Point2d & directionSample, float time) override;

    LightSampleResult sampleDirect(const Intersection & its, const Point2d & sample, float time) override;

    LightSampleResult sampleDirect(const MediumSampleRecord & mRec, Point2d sample, double time) override;
protected:
    double  directPdf(Vec3d dir);

    Spectrum _emission;

    Vec3d _capDir;
    double _capAngle;
    double _cosCapAngle;

    Vec3d _worldCenter;
    double _worldRadius;

    Frame _capFrame;
};