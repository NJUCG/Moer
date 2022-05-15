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
#include "../Core/CoreLayer/Matrix.h"

class PointLight : public Light
{
    TransformMatrix3D transform;
    Spectrum intensity;

public:
    PointLight(const Spectrum &intensity, const TransformMatrix3D &transform);
    virtual LightSampleResult eval(const Ray &ray) override;
    virtual LightSampleResult sampleEmit(const Point2f &positionSample, const Point2f &directionSample, float time) override;
    virtual LightSampleResult sampleDirect(const Point2f &sample, float time) override;
};