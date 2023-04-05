/**
 * @file Beerslaw.h
 * @author Chenxi Zhou
 * @brief Medium only absorbtion
 * @version 0.1
 * @date 2022-09-23
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#pragma once

#include "Medium.h"

class BeerslawMedium : public Medium {
public:

    BeerslawMedium(const Spectrum &density, std::shared_ptr<PhaseFunction> _phase) 
        : Medium(_phase), mDensity(density) { }

    virtual bool sampleDistance(MediumSampleRecord *mRec,
                                const Ray &ray,
                                const Intersection &its,
                                Point2d sample) const;

    virtual Spectrum evalTransmittance(Point3d from, Point3d dest) const;
private:
    Spectrum mDensity;
};