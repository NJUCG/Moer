/**
 * @file PointLight.h
 * @author Zhimin Fan
 * @brief Point light.
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../Core/FunctionLayer/Light.h"
#include "../Core/FunctionLayer/Transform3D.h"

class PointLight : public Light
{
protected:
    std::shared_ptr<Transform3D> transform;
    Spectrum intensity;

public:
    PointLight(const Spectrum &intensity, const std::shared_ptr<Transform3D> &transform);
    virtual LightSampleResult evalEnvironment(const Ray &ray) override;
	virtual LightSampleResult eval(const Intersection &its, const Vec3d &d) override;
    virtual LightSampleResult sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time) override;
    virtual LightSampleResult sampleDirect(const Intersection& its, const Point2d &sample, float time) override;
};