/**
 * @file  Mirror.h
 * @author Junping Yuan
 * @brief   Mirror Material
 * @version 0.1
 * @date 2022/6/8
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "CoreLayer/Geometry/Frame.h"
#include "BxDF.h"

#pragma  once

class Mirror : public  BxDF {

    virtual Spectrum f(const Vec3d &wo, const Vec3d &wi) const;

    virtual Vec3d sampleWi(const Vec3d &wo, const Point2d &sample) const;

    virtual double pdf(const Vec3d &wo, const Vec3d &wi) const;

    virtual BxDFSampleResult sample(const Vec3d &wo, const Point2d &sample) const;

    virtual bool isSpecular() const;
};

