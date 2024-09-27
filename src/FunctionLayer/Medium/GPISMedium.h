#pragma once

#include "Medium.h"
#include "CoreLayer/Adapter/JsonUtil.h"
#include "FunctionLayer/GaussianProcess/GaussianProcess.h"

class GPISMedium : public Medium {
public:
    GPISMedium() = default;
    GPISMedium(const Json &json);

    virtual bool sampleDistance(MediumSampleRecord *mRec,
                                const Ray &ray,
                                const std::optional<Intersection> &its,
                                Point2d sample) const override;

    virtual Spectrum evalTransmittance(Point3d from,
                                       Point3d dest) const override;

    bool intersectGP(const Ray &ray, GPRealization &gpRealization, double &t, Sampler &sampler) const;

private:
    std::shared_ptr<GaussianProcess> gaussianProcess;
    int numSamplePoints = 8;
    double marchingStepSize = 0.;

    MemoryModel memoryModel = MemoryModel::RenewalPlus;
};