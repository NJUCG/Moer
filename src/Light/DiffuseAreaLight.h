/**
 * @file DiffuseAreaLight.h
 * @author Zhimin Fan
 * @brief Diffuse Area light.
 * @version 0.1
 * @date 2022-05-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "AreaLight.h"
#include "../Core/FunctionLayer/Transform3d.h"
#include "../Core/FunctionLayer/Entity.h"

class DiffuseAreaLight : public AreaLight
{
    std::shared_ptr<Entity> shape;
    Spectrum radiance;

public:
    DiffuseAreaLight(std::shared_ptr<Entity> shape,
                     Spectrum radiance,
                     std::shared_ptr<Transform3D> transform3D);
    virtual LightSampleResult evalEnvironment(const Ray &ray) override;
	virtual LightSampleResult eval(const Ray& ray, const Intersection &its, const Vec3d &d) override;
    virtual LightSampleResult sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time) override;
    virtual LightSampleResult sampleDirect(const Intersection &its, const Point2d &sample, float time) override;
};