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

struct MajorantTracker {
public:
    bool track(int index[3], float *dt, int *stepAxis = nullptr);

public:
    bool terminate = false;
    float tmin, tmax;
    float nextCrossingT[3], deltaT[3];
    int step[3], voxelLimit[3], voxel[3];
};

struct MajorantGrid {
public:
    MajorantGrid() = default;

    MajorantGrid(int _resolution[3], Point3d boxMin, Point3d boxMax,
                 float voxelScale)
        : box(boxMin, boxMax), voxelScale(voxelScale) {
        resulotion[0] = _resolution[0];
        resulotion[1] = _resolution[1];
        resulotion[2] = _resolution[2];
        majorantVoxel =
            std::vector<float>(resulotion[0] * resulotion[1] * resulotion[2]);
        voxelSize[0] = (boxMax[0] - boxMin[0]) / resulotion[0];
        voxelSize[1] = (boxMax[1] - boxMin[1]) / resulotion[1];
        voxelSize[2] = (boxMax[2] - boxMin[2]) / resulotion[2];
    }

    float at(int x, int y, int z) const {
        int offset = x + y * resulotion[0] + z * resulotion[1] * resulotion[0];
        return majorantVoxel[offset];
    }

    void set(int x, int y, int z, float val) {
        int offset = x + y * resulotion[0] + z * resulotion[1] * resulotion[0];
        majorantVoxel[offset] = val;
    }

    MajorantTracker getTracker(Point3d origin_u, Vec3d dir_u,
                               float t_grid) const;

public:
    BoundingBox3f box;
    int resulotion[3];
    float voxelSize[3], voxelScale;
    std::vector<float> majorantVoxel;
};

// TODO Handle chromatic heterogeneous
class HeterogeneousMedium : public Medium {
public:
    HeterogeneousMedium(std::string gridFilePath, std::shared_ptr<PhaseFunction> phase);

    virtual bool sampleDistance(MediumSampleRecord *mRec,
                                const Ray &ray,
                                const Intersection &its,
                                Point2d sample) const override;

    virtual Spectrum evalTransmittance(Point3d from,
                                       Point3d dest) const override;

    virtual bool sampleDistanceDeltaTracking(MediumSampleRecord *mRec,
                                             const Ray &ray,
                                             const Intersection &itsOpt,
                                             Point2d sample) const override;

    virtual Spectrum evalTransmittanceRatioTracking(Point3d from, Point3d dest) const override;

protected:
    double scaleSample(nanovdb::Vec3R index,
                       const nanovdb::FloatGrid *grid) const;

public:
    Point3d boxMin, boxMax;

private:
    float voxelSize;
    int minIndex[3], maxIndex[3];

    float sigmaScale = 1.f;

    nanovdb::GridHandle<BufferT> densityGrid;
    nanovdb::GridHandle<BufferT> temperatureGrid;

    const nanovdb::FloatGrid *densityFloatGrid = nullptr;
    const nanovdb::FloatGrid *temperatureFloatGrid = nullptr;

    MajorantGrid majorantGrid;
};