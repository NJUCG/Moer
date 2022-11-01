/**
 * @file PointLight.cpp
 * @author Zhimin Fan
 * @brief Point light.
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "PointLight.h"
#include "CoreLayer/Geometry/Angle.h"
#include "FunctionLayer/Integrator/AbstractPathIntegrator.h"

constexpr double DIRAC = 1.0;

PointLight::PointLight(const Spectrum &intensity, const Point3d &center) :Light(ELightType::POINT) ,intensity(intensity)
{
    Transform3D::setTranslate(center.x, center.y, center.z);
}

LightSampleResult PointLight::evalEnvironment(const Ray &ray)
{
    LightSampleResult ans;
    ans.s = 0.0;
    ans.src = ray.origin;
    ans.dst = Transform3D::getTranslate();
    ans.wi = normalize(ans.dst - ans.src);
    ans.pdfDirect = 0.0;
    ans.pdfEmitPos = 0.0;
    ans.pdfEmitDir = 0.0;
    ans.isDeltaPos = true;
    ans.isDeltaDir = false;
    return ans;
}

LightSampleResult PointLight::eval(const Ray &ray, const Intersection &its, const Vec3d &d)
{
    // This function should not be called.
    LightSampleResult ans;
    ans.s = 0.0;
    ans.src = ray.origin;
    ans.dst = its.position;
    ans.wi = d;
    ans.pdfDirect = 0.0;
    ans.pdfEmitPos = 0.0;
    ans.pdfEmitDir = 0.0;
    ans.isDeltaPos = true;
    ans.isDeltaDir = false;
    return ans;
}

LightSampleResult PointLight::sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time)
{
    Polar3d polar=Polar3d(1.0,Angle(directionSample.x * 2 * M_PI,Angle::EAngleType::ANGLE_RAD),Angle(acos(directionSample.y*2.0-1.0),Angle::EAngleType::ANGLE_RAD));
    Normal3d wi = polar.toVec3d();
    LightSampleResult ans;
    ans.s = intensity * DIRAC;
    ans.dst = Transform3D::getTranslate();
    ans.wi = wi;
    ans.pdfEmitPos = 1.0 * DIRAC;
    ans.pdfEmitDir = 1.0 / M_PI / 4;
    ans.pdfDirect = 0.0;
    ans.isDeltaPos = true;
    ans.isDeltaDir = false;
    return ans;
}

LightSampleResult PointLight::sampleDirect(const Intersection &its, const Point2d &sample, float time)
{
    Normal3d wi = normalize(Transform3D::getTranslate() - its.position);
    LightSampleResult ans;
    ans.src = its.position;
    ans.dst = Transform3D::getTranslate();
    ans.s = intensity / (ans.dst - ans.src).length2() * DIRAC;
    ans.wi = wi;
    ans.pdfDirect = 1.0 * DIRAC;
    ans.isDeltaPos = true;
    ans.isDeltaDir = false;
    return ans;
}

LightSampleResult PointLight::sampleDirect(const MediumSampleRecord &mRec,
                                           Point2d sample,
                                           double time) 
{
    Normal3d wi = normalize(Transform3D::getTranslate() - mRec.scatterPoint);
    LightSampleResult ans;
    ans.src = mRec.scatterPoint;
    ans.dst = Transform3D::getTranslate();
    ans.s = intensity / (ans.dst - ans.src).length2() * DIRAC;
    ans.wi = wi;
    ans.pdfDirect = 1.0 * DIRAC;
    ans.isDeltaPos = true;
    ans.isDeltaDir = false;
    return ans;
}


void PointLight::apply()
{
}
