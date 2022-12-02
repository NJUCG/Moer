/**
 * @file Homogeneous.h
 * @author Chenxi Zhou
 * @brief Homogeneous medium
 * @version 0.1
 * @date 2022-09-24
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#pragma once

#include "Medium.h"

class HomogeneousMedium : public Medium {
public:

    HomogeneousMedium(double density, double albedo, 
                      std::shared_ptr<PhaseFunction> phase)
        :Medium(phase), mDensity(density), mAlbedo(albedo) { }

    virtual bool sampleDistance(MediumSampleRecord *mRec,
                                const Ray &ray,
                                const std::optional<Intersection> &its,
                                Point2d sample) const override;

    virtual Spectrum evalTransmittance(Point3d from, 
                                       Point3d dest) const override;

private:
    //* sigma_s = mAlbedo * mDensity
    //* sigma_a = (1 - mAlbedo) * mDensity
    
    //TODO replace with spectrum
    double mDensity;
    double mAlbedo;
};