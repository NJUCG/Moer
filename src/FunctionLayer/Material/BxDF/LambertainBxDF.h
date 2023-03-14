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
#include "CoreLayer/Geometry/Frame.h"
#include "BxDF.h"

/// \brief Lambertian reflection model
/// \ingroup BxDF
class LambertainBxDF : public  BxDF{
private:

    Spectrum  albedo;

public:

    LambertainBxDF(Spectrum albedo);

    virtual Spectrum f(const Vec3d &wo, const Vec3d &wi) const;

    virtual double pdf(const Vec3d &wo, const Vec3d &wi) const;

    virtual BxDFSampleResult sample(const Vec3d &wo, const Point2d &sample) const;

    [[nodiscard]]
    double getRoughness() const override { return 1; }

};



