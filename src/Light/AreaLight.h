/**
 * @file AreaLight.h
 * @author Zhimin Fan
 * @brief Area light (abstract).
 * @version 0.1
 * @date 2022-05-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../Core/FunctionLayer/Light.h"
#include "../Core/FunctionLayer/Transform3D.h"

class AreaLight : public Light
{
protected:
    std::shared_ptr<Transform3D> transform3D;

public:
    AreaLight(std::shared_ptr<Transform3D> transform3D);
    virtual LightSampleResult eval(const Ray &ray) = 0;
    virtual LightSampleResult sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time) = 0;
    virtual LightSampleResult sampleDirect(const Intersection& its, const Point2d &sample, float time) = 0;
};