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
class HeterogeneousMedium : public Medium {
public:
    HeterogeneousMedium(std::shared_ptr<PhaseFunction> phase);

    virtual bool sampleDistance(MediumSampleRecord *mRec,
                                const Ray &ray,
                                const Intersection &its,
                                Point2d sample) const override;

    virtual Spectrum evalTransmittance(Point3d from,
                                       Point3d dest) const override;

public:
    Point3d boxMin, boxMax;

private:
    nanovdb::GridHandle<BufferT> densityGrid;
    nanovdb::GridHandle<BufferT> temperatureGrid;

    const nanovdb::FloatGrid *densityFloatGrid = nullptr;
    const nanovdb::FloatGrid *temperatureFloatGrid = nullptr;
};