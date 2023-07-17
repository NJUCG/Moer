/**
* @file
* @author Junping Yuan
* @brief
* @version 0.1
* @date
*
* @copyright NJUMeta (c) 2022
* www.njumeta.com
*
*/

#include "LambertainBxDF.h"

LambertainBxDF::LambertainBxDF(Spectrum albedo) : albedo(albedo) {
}


Spectrum LambertainBxDF::f(const Vec3d & wo , const Vec3d & wi) const {
   if(wi.z<0 || wo.z<0)
       return 0;
   return albedo * INV_PI;
}

double LambertainBxDF::pdf(const Vec3d &wo, const Vec3d &wi) const {
   return SquareToUniformHemispherePdf(wi);
}

BxDFSampleResult LambertainBxDF::sample(const Vec3d  & wo, const Point2d &sample) const {
   BxDFSampleResult result ;
   auto wi = SquareToUniformHemisphere(sample);
   result.directionIn = wi;
   result.bxdfSampleType = BXDFType(BXDF_DIFFUSE | BXDF_REFLECTION);
   result.pdf = pdf(wo, wi);
   result.s = f(wo, wi);

   return result;
}
