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
    HomogeneousMedium(Spectrum sigmaT, Spectrum albedo,
                      std::shared_ptr<PhaseFunction> phase)
        : Medium(phase), mSigmaT(sigmaT), mAlbedo(albedo), mSigmaS(mAlbedo * mSigmaT), mSigmaA(mSigmaT - mSigmaS) {}

    virtual bool sampleDistance(MediumSampleRecord *mRec,
                                const Ray &ray,
                                const Intersection &its,
                                Point2d sample) const override;

    virtual Spectrum evalTransmittance(Point3d from,
                                       Point3d dest) const override;

private:
    //* sigma_s = mAlbedo * mDensity
    //* sigma_a = (1 - mAlbedo) * mDensity
    Spectrum mSigmaT;
    Spectrum mAlbedo;
    Spectrum mSigmaS;
    Spectrum mSigmaA;
};