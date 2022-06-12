/**
 * @file  Diffuse.h
 * @author Junping Yuan
 * @brief diffuse bxdf
 * @version 0.1
 * @date 2022/06/06
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma  once
#include "../Core/FunctionLayer/BxDF.h"
#include "Core/FunctionLayer/Frame.h"



class Diffuse : public  BxDF{
private:

    Spectrum  albedo;

public:

    Diffuse(Spectrum albedo);

    virtual Spectrum f(const Vec3d &wo, const Vec3d &wi) const;

    virtual Vec3d sampleWi(const Vec3d &wo, const Point2d &sample) const;

    virtual double pdf(const Vec3d &wo, const Vec3d &wi) const;

    virtual BxDFSampleResult sample(const Vec3d &wo, const Point2d &sample) const;

    virtual bool isSpecular() const;

};



