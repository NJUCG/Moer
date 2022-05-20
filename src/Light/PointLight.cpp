/**
 * @file PointLight.cpp
 * @author Zhimin Fan
 * @brief Point light.
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "PointLight.h"

#define DIRAC 1.0 // todo: delete it

LightSampleResult PointLight::eval(const Ray &ray)
{
    // Fill s, src, dst, wi, pdf, pdfP, pdfD, isDP, isDD
    LightSampleResult ans;
    ans.s = 0.0;
    ans.src = ray.origin;
    ans.dst = transform->getTranslate();
    ans.wi = normalize(ans.dst - ans.src);
    ans.pdf = 0.0;
    ans.pdfPos = 0.0;
    ans.pdfDir = 0.0;
    ans.isDeltaPos = true;
    ans.isDeltaDir = false;
    return ans;
}

LightSampleResult PointLight::sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time)
{
    Normal3d wi; // todo: convert directionSample to wi
    // Fill s, src, dst, wi, pdf, pdfP, pdfD, isDP, isDD
    LightSampleResult ans;
    ans.s = intensity * DIRAC;
    ans.dst = transform->getTranslate();
    ans.wi = wi;
    ans.pdfPos = 1.0 * DIRAC;
    ans.pdfDir = 1.0 / 3.1415926 / 4; // todo: replace with a constant pi
    ans.pdf = ans.pdfPos * ans.pdfDir;
    ans.isDeltaPos = true;
    ans.isDeltaDir = false;
    return ans;
}

LightSampleResult PointLight::sampleDirect(const Intersection &its, const Point2d &sample, float time)
{
    Normal3d wi = normalize(transform->getTranslate() - its.position);
    // Fill s, src, dst, wi, pdf, pdfP, pdfD, isDP, isDD
    LightSampleResult ans;
    ans.src = its.position;
    ans.dst = transform->getTranslate();
    ans.s = intensity / (ans.dst - ans.src).length2() * DIRAC;
    ans.wi = wi;
    ans.pdf = 1.0 * DIRAC;
    ans.isDeltaPos = true;
    ans.isDeltaDir = false;
    return ans;
}