#include "Beerslaw.h"

bool BeerslawMedium::sampleDistance(MediumSampleRecord *mRec,
                                    const Ray &ray,
                                    const std::optional<Intersection> &its,
                                    Point2d sample) const {
    //* No scattering, only absorbtion
    double maxT = ray.timeMax;
    if (its) {
        maxT = std::min(maxT, its.value().t);
    }

    mRec->marchLength = maxT;
    mRec->pdf = 1;

    mRec->tr = evalTransmittance(ray.origin, ray.origin + ray.direction * maxT);
    return false;
}

Spectrum BeerslawMedium::evalTransmittance(Point3d from,
                                           Point3d end) const {
    double distance = (end - from).length();
    return exp(mDensity * -distance);
}