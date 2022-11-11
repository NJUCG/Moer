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


    virtual double pdf(const Vec3d &out, const Vec3d &in) const = 0;

    BxDFSampleResult sample(const Vec3d & out,const Point2d & sample,bool adjoint){
        BxDFSampleResult result = this->sample(out,sample);
        if(!adjoint){
            result.s *= pow(eta(out,result.directionIn),2);
        }
        else {

        }
        return result;
    }
    Spectrum f(const Vec3d & out,const Vec3d & in,bool adjoint) {
        Spectrum result = f(out,in);
        if(!adjoint){
            result *= pow(eta(out,in),2);
        }
        else {

        }
        return result;
    }
protected:
    virtual BxDFSampleResult sample(const Vec3d &out, const Point2d& sample) const = 0;
    virtual Spectrum f(const Vec3d &out, const Vec3d &in) const = 0;
    virtual double eta(const Vec3d &out,const Vec3d & in) const {return 1;}
    BXDFType type;
};

// BSDF Inline Functions
inline double CosTheta(const Vec3d &w) { return w.z; }
inline double Cos2Theta(const Vec3d &w) { return w.z * w.z; }
inline double AbsCosTheta(const Vec3d &w) { return std::abs(w.z); }
inline double Sin2Theta(const Vec3d &w) {
    return std::max((double)0, (double)1 - Cos2Theta(w));
}

inline double SinTheta(const Vec3d &w) { return std::sqrt(Sin2Theta(w)); }

inline double TanTheta(const Vec3d &w) { return SinTheta(w) / CosTheta(w); }

inline double Tan2Theta(const Vec3d &w) {return Sin2Theta(w) / Cos2Theta(w);}

inline double CosPhi(const Vec3d &w) {
    double sinTheta = SinTheta(w);
    return (sinTheta == 0) ? 1 : std::clamp(w.x / sinTheta, -1.0, 1.0);
}

inline double SinPhi(const Vec3d &w) {
    double sinTheta = SinTheta(w);
    return (sinTheta == 0) ? 0 : std::clamp(w.y / sinTheta, -1.0, 1.0);
}

inline double Cos2Phi(const Vec3d &w) { return CosPhi(w) * CosPhi(w); }

inline double Sin2Phi(const Vec3d &w) { return SinPhi(w) * SinPhi(w); }

