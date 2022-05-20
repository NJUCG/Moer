/**
 * @file DiffuseAreaLight.cpp
 * @author Zhimin Fan
 * @brief Diffuse Area light impl.
 * @version 0.1
 * @date 2022-05-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "DiffuseAreaLight.h"

LightSampleResult DiffuseAreaLight::eval(const Ray &ray)
{
    // Fill s, src, dst, wi, pdf, pdfP, pdfD, isDP, isDD
    LightSampleResult ans;
    // todo
    return ans;
}

LightSampleResult DiffuseAreaLight::sampleEmit(const Point2d &positionSample, const Point2d &directionSample, float time)
{
    // Fill s, src, dst, wi, pdf, pdfP, pdfD, isDP, isDD
    LightSampleResult ans;
    // todo
    return ans;
}

LightSampleResult DiffuseAreaLight::sampleDirect(const Intersection &its, const Point2d &sample, float time)
{
    // Fill s, src, dst, wi, pdf, pdfP, pdfD, isDP, isDD
    LightSampleResult ans;
    // todo
    return ans;
}