/**
 * @file  Mirror.h
 * @author Junping Yuan
 * @brief   Mirror bxdf
 * @version 0.1
 * @date 2022/6/8
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma  once
#include "CoreLayer/Geometry/Frame.h"
#include "BxDF.h"

/// \brief Ideally specular reflection model
/// \ingroup BxDF
class MirrorBxDF : public  BxDF {

    virtual Spectrum f(const Vec3d &wo, const Vec3d &wi) const;

    virtual double pdf(const Vec3d &wo, const Vec3d &wi) const;

    virtual BxDFSampleResult sample(const Vec3d &wo, const Point2d &sample) const;

    [[nodiscard]]
    double getRoughness() const override { return 0; }

};

