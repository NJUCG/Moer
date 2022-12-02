#include "Homogeneous.h"

bool HomogeneousMedium::sampleDistance(MediumSampleRecord *mRec, 
                                       const Ray &ray, 
                                       const std::optional<Intersection> &its, 
                                       Point2d sample) const 
{
    auto [x, y] = sample;
    //TODO sample a channel first
    //! Might crush becaust bad optional access

    double dist = -std::log(1 - x) / mDensity;

    if (dist < its->t) {
        //* Scatter in medium
        mRec->marchLength = dist;
        mRec->scatterPoint = ray.at(dist);
        mRec->pdf = mDensity * std::exp(-mDensity * dist);
        mRec->sigmaA = Spectrum{(1 - mAlbedo) * mDensity};
        mRec->sigmaS = Spectrum{mAlbedo * mDensity};
        mRec->tr = evalTransmittance(ray.origin, mRec->scatterPoint);
        return true;
    } else {
        //* Pass through this medium without collision
        mRec->marchLength = its->t;
        mRec->pdf = std::exp(-mDensity * its->t);
        mRec->tr = evalTransmittance(ray.origin, its->position);
        return false;
    }
}

Spectrum HomogeneousMedium::evalTransmittance(Point3d from, 
                                              Point3d dest) const 
{
    double dist = (dest - from).length();
    return Spectrum{std::exp(-mDensity * dist)};
}