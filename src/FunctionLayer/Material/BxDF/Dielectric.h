/**
 * @file  Dielectric.h
 * @author Junping Yuan
 * @brief  Dielectric Bxdf
 * @version 0.1
 * @date
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma  once
#include "CoreLayer/Geometry/Frame.h"
#include "BxDF.h"
class Dielectric : public  BxDF{
public:
    Dielectric(float m_intIOR =1.5046f,float m_extIOR = 1.00277f) : m_intIOR(m_intIOR), m_extIOR(m_extIOR){

    }

    virtual Spectrum f(const Vec3d &wo, const Vec3d &wi) const ;

    virtual Vec3d sampleWi(const Vec3d &wo, const Point2d &sample) const ;

    virtual double pdf(const Vec3d &wo, const Vec3d &wi) const ;


    virtual BxDFSampleResult sample(const Vec3d &wo, const Point2d &sample) const ;

    virtual bool isSpecular() const ;


private:
   float m_intIOR;
   float m_extIOR;

    static Vec3d reflect(const Vec3d &wi) {
        return Vec3d {
                -wi[0], -wi[1], wi[2]
        };
    }

    Vec3d refract(const Vec3d &wi) const {
        float cosThetaI = Frame::cosTheta(wi),
                eta = cosThetaI > 0 ? m_extIOR / m_intIOR : m_intIOR / m_extIOR,
                cosThetaT = std::sqrt(
                1 - eta*eta*(1-cosThetaI*cosThetaI)
        );
        return Vec3d {
                - wi[0] * eta,
                - wi[1] * eta,
                cosThetaI > 0 ? -cosThetaT : cosThetaT
        };
    }


};


