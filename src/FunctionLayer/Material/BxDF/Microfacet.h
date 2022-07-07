/**
 * @file  Mircofacet.h
 * @author Junping Yuan 
 * @brief  Microfacet model
 * @version 0.1
 * @date 2022/6/12
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma  once

#include "BxDF.h"
#include "CoreLayer/Geometry/Frame.h"

class Mircofacet : public  BxDF {


public:
    Mircofacet(Spectrum kd, double intIOR, double extIOR,double alpha);

    virtual Spectrum f(const Vec3d &wo, const Vec3d &wi) const;

    virtual Vec3d sampleWi(const Vec3d &wo, const Point2d &sample) const;

    virtual double pdf(const Vec3d &wo, const Vec3d &wi) const;

    virtual BxDFSampleResult sample(const Vec3d &wo, const Point2d &sample) const;

    virtual bool isSpecular() const;
private:
    Spectrum kd;
    double ks;
    double intIOR,extIOR,alpha;
};


