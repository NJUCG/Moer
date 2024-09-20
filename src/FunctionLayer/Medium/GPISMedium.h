#pragma once

#include "Medium.h"

class GPISMedium : public Medium {
public:
    GPISMedium() = default;

    virtual bool sampleDistance(MediumSampleRecord *mRec,
                                const Ray &ray,
                                const Intersection &its,
                                Point2d sample) const override;

    virtual Spectrum evalTransmittance(Point3d from,
                                       Point3d dest) const override;

private:
    
};