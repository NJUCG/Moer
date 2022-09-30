/**
 * @file PointLight.h
 * @author Zhimin Fan
 * @brief Point light.
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma once

#include "Light.h"
#include "CoreLayer/Geometry/Transform3d.h"

/// \brief Point light source
/// \ingroup Light
class PointLight : public Light, public Transform3D
{
protected:
    Spectrum intensity;

public:
    PointLight(const Spectrum &intensity, const Point3d &center);
    virtual void apply() override;
    virtual LightSampleResult evalEnvironment(const Ray &ray) override;
    virtual LightSampleResult eval(const Ray &ray, const Intersection &its, const Vec3d &d) override;
    virtual LightSampleResult sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time) override;
    virtual LightSampleResult sampleDirect(const Intersection &its, const Point2d &sample, float time) override;

	virtual LightSampleResult sampleDirect(const MediumSampleRecord &mRec,
										   Point2d sample,
										   double time) override;
};