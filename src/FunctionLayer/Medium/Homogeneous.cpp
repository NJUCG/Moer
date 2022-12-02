#include "Homogeneous.h"
#include <Random>

bool HomogeneousMedium::sampleDistance(MediumSampleRecord *mRec, 
                                       const Ray &ray, 
                                       const std::optional<Intersection> &its, 
                                       Point2d sample) const 
{
    auto [x, y] = sample;
    //TODO sample a channel first
    //! Might crush becaust bad optional access

    // randomly (and uniformly) pick a channel

    std::random_device rd;
    std::uniform_int_distribution<int> dist(0,nSpectrumSamples);
    int channelIndex=dist(rd);

    // sample a distance on single channel

    double dist = -std::log(1 - x) / sigmaT[channelIndex];

    // calculate pdf

    double pdf=0.0,tCurrent=std::min(dist,its.t);
    Spectrum coeff(1.0);
    // coeff == sigmaT if sampled medium, else coeff==1.0
    if(dist<its->t) 
        coeff=sigmaT;
    for(int i=0;i<nSpectrumSamples;i++) 
        pdf+=coeff[i]*exp(-sigmaT[i]*tCurrent);
    pdf/=nSpectrumSamples;

    if (dist < its->t) {
        //* Scatter in medium
        mRec->marchLength = dist;
        mRec->scatterPoint = ray.at(dist);
        mRec->pdf = pdf;
        mRec->sigmaA = sigmaA;
        mRec->sigmaS = sigmaS;
        mRec->tr = evalTransmittance(ray.origin, mRec->scatterPoint);
        return true;
    } else {
        //* Pass through this medium without collision
        mRec->marchLength = its->t;
        mRec->pdf = pdf;
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