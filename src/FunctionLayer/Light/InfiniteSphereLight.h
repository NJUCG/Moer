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

#include "Light.h"

#include "FunctionLayer/Texture/ImageTexture.h"
#include "FunctionLayer/Distribution/Distribution.h"


class InfiniteSphereLight : public  Light{
public:
    InfiniteSphereLight(const Json & json);

    LightSampleResult evalEnvironment(const Ray & ray) override;

    LightSampleResult eval(const Ray & ray, const Intersection & its, const Vec3d & d) override;

    LightSampleResult sampleEmit(const Point2d & positionSample, const Point2d & directionSample, float time) override;

    LightSampleResult sampleDirect(const Intersection & its, const Point2d & sample, float time) override;

    LightSampleResult sampleDirect(const MediumSampleRecord & mRec, Point2d sample, double time) override;
protected:
    std::unique_ptr <Distribution2D> distribution;
    std::shared_ptr <ImageTexture<Spectrum,RGB3>> emission;
};