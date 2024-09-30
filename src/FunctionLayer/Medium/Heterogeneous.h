/**
 * @file Heterogeneous.h
 * @author Chenxi Zhou
 * @brief 
 * @version 0.1
 * @date 2023-08-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <nanovdb/NanoVDB.h>
#include <nanovdb/util/GridHandle.h>
#include "Medium.h"
#include <CoreLayer/Geometry/Matrix.h>
using BufferT = nanovdb::HostBuffer;

class HeterogeneousMedium : public Medium {
public:
    HeterogeneousMedium(std::string gridFilePath, std::shared_ptr<PhaseFunction> phase, TransformMatrix3D _transform, float _sigmaScale = 1.f);

    virtual bool sampleDistance(MediumSampleRecord *mRec, const Ray &ray, const Intersection &its, Point2d sample) const override;

    virtual Spectrum evalTransmittance(Point3d from, Point3d dest) const override;

private:
protected:
    double scaleSample(nanovdb::Vec3R index,
                       const nanovdb::FloatGrid *grid) const;

    Point3d worldToIndex(Point3d world) const;

    Point3d indexToWorld(Point3d index) const;

    Vec3d worldToIndexDir(Vec3d world) const;

    Vec3d indexToWorldDir(Vec3d index) const;

private:
    mutable TransformMatrix3D transformMatrix, invTransformMatrix;

    float voxelSize;
    float sigmaScale = 1.f;
    int minIndex[3], maxIndex[3];

    nanovdb::GridHandle<BufferT> densityGrid;
    nanovdb::GridHandle<BufferT> temperatureGrid;

    const nanovdb::FloatGrid *densityFloatGrid = nullptr;
    const nanovdb::FloatGrid *temperatureFloatGrid = nullptr;
};