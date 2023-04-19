/**
 * @file Heterogeneous.h
 * @author Chenxi Zhou
 * @brief 
 * @version 0.1
 * @date 2023-04-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <nanovdb/NanoVDB.h>
#include <nanovdb/util/GridHandle.h>
#include "Medium.h"

using BufferT = nanovdb::HostBuffer;

// TODO Handle chromatic heterogeneous
class HeterogeneousMedium : public Medium {
public:
    HeterogeneousMedium(std::shared_ptr<PhaseFunction> phase);

    virtual bool sampleDistance(MediumSampleRecord *mRec,
                                const Ray &ray,
                                const Intersection &its,
                                Point2d sample) const override;

    virtual Spectrum evalTransmittance(Point3d from,
                                       Point3d dest) const override;

protected:
    float scaleSample(nanovdb::Vec3R index,
                      const nanovdb::FloatGrid *grid) const;

public:
    Point3d boxMin,
        boxMax;

private:
    float voxelSize;
    int minIndex[3], maxIndex[3];

    float sigmaScale = 1.f;

    nanovdb::GridHandle<BufferT> densityGrid;
    nanovdb::GridHandle<BufferT> temperatureGrid;

    const nanovdb::FloatGrid *densityFloatGrid = nullptr;
    const nanovdb::FloatGrid *temperatureFloatGrid = nullptr;
};