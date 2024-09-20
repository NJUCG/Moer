#include "GPISMedium.h"

bool GPISMedium::sampleDistance(MediumSampleRecord *mRec, const Ray &ray, const Intersection &its, Point2d sample) const {
    return false;
}

Spectrum GPISMedium::evalTransmittance(Point3d from, Point3d dest) const {
    return Spectrum();
}
