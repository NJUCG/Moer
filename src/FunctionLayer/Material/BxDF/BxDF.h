/**
 * @file BxDF.h
 * @author orbitchen
 * @brief BxDF, including BRDF and BTDF.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "CoreLayer/ColorSpace/Color.h"
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Math/Warp.h"

enum BXDFType {
    BXDF_REFLECTION = 1 << 0,
    BXDF_TRANSMISSION = 1 << 1,
    BXDF_DIFFUSE = 1 << 2,
    BXDF_GLOSSY = 1 << 3,
    BXDF_SPECULAR = 1 << 4,
    BXDF_ALL = BXDF_DIFFUSE | BXDF_GLOSSY | BXDF_SPECULAR | BXDF_REFLECTION |
               BXDF_TRANSMISSION,
};

struct BxDFSampleResult
{
    Spectrum s;
    Vec3d directionIn;
    double pdf;
    BXDFType bxdfSampleType;
};


/// \defgroup BxDF

/// @brief BxDF. out == rays from/to camera, in == rays from/to objects/lights.
/// \ingroup BxDF
/// \todo Use a structure warp the BxDF query information
class BxDF
{

public:
    static bool MatchFlags(BXDFType typeToMatch,BXDFType type ){
        return (typeToMatch & type) == type;
    }

	virtual Spectrum f(const Vec3d &out, const Vec3d &in) const = 0;

	virtual Vec3d sampleWi(const Vec3d &out, const Point2d& sample) const = 0;

	virtual double pdf(const Vec3d &out, const Vec3d &in) const = 0;

	virtual BxDFSampleResult sample(const Vec3d &out, const Point2d& sample) const = 0;

	virtual bool isSpecular() const = 0;
protected:
    BXDFType type;
};