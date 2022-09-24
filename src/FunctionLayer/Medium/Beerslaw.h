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
    BeerslawMedium() = default;

    BeerslawMedium(const Spectrum &density) : mDensity(density) { }

    virtual bool sampleDistance(MediumSampleRecord *mRec,
                                const Ray &ray,
                                const std::optional<Intersection> &its,
                                Point2d sample) const;

    virtual Spectrum evalTransmittance(Point3d from, Point3d end) const;
private:
    Spectrum mDensity;
};