#include "Beerslaw.h"

bool BeerslawMedium::sampleDistance(MediumSampleRecord *mRec,
                                    const Ray &ray, 
                                    const Intersection &its, 
                                    Point2d sample) const 
{
    //* No scattering, only absorbtion

    mRec->marchLength = its.t;
    mRec->pdf = 1;

    mRec->tr = evalTransmittance(ray.origin, its.position);
    return false;    
}

Spectrum BeerslawMedium::evalTransmittance(Point3d from, 
                                           Point3d end) const
{
    double distance = (end - from).length();
    return exp(mDensity * -distance);
} 