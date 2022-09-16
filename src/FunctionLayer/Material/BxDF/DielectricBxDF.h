/**
 * @file  Dielectric.h
 * @author Junping Yuan
 * @brief  Dielectric Bxdf
 * @version 0.1
 * @date
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma  once
#include "CoreLayer/Geometry/Frame.h"
#include "BxDF.h"

/// \brief Dielectric reflection model
/// \ingroup BxDF
class DielectricBxDF : public  BxDF{
public:
    DielectricBxDF(double ior = 1.33) : ior(ior),invIor(1/ior){

    }

    virtual Spectrum f(const Vec3d &wo, const Vec3d &wi) const ;

    virtual Vec3d sampleWi(const Vec3d &wo, const Point2d &sample) const ;

    virtual double pdf(const Vec3d &wo, const Vec3d &wi) const ;


    virtual BxDFSampleResult sample(const Vec3d &wo, const Point2d &sample) const ;

    virtual bool isSpecular() const ;


private:
   double ior;
   double invIor;
   Spectrum albedo;
};


