#include "Heterogeneous.h"
#include <nanovdb/util/IO.h>
#include <nanovdb/util/SampleFromVoxels.h>
#include <FunctionLayer/Sampler/Independent.h>
#include <ResourceLayer/File/FileUtils.h>

class RegularTracker {
public:
    RegularTracker() = delete;

    RegularTracker(const int min[3], const int max[3], Point3d origin, Vec3d direction, float _tmax, float _voxelSize) {
        voxelSize = _voxelSize;
        tmin = .0f;
        tmax = _tmax / _voxelSize;

        for (int axis = 0; axis < 3; ++axis) {
            voxel[axis] = clamp((int)std::floor(origin[axis]), min[axis], max[axis]);
            deltaT[axis] = 1.f / std::abs(direction[axis]);

            if (direction[axis] == -.0f) direction[axis] = .0f;
            if (direction[axis] >= 0) {
                nextCrossingT[axis] = (voxel[axis] + 1.f - origin[axis]) / direction[axis];
                step[axis] = 1;
                voxelLimit[axis] = max[axis] + 1;
            } else {
                nextCrossingT[axis] = (voxel[axis] - origin[axis]) / direction[axis];
                step[axis] = -1;
                voxelLimit[axis] = min[axis] - 1;
            }
        }
    }

    bool track(int index[3], float *dt) {
        if (terminate) return false;

        int stepAxis = -1;

        if (nextCrossingT[0] < nextCrossingT[1] && nextCrossingT[0] < nextCrossingT[2])
            stepAxis = 0;
        else if (nextCrossingT[1] < nextCrossingT[2])
            stepAxis = 1;
        else
            stepAxis = 2;

        if (nextCrossingT[stepAxis] > tmax) {
            *dt = (tmax - tmax) * voxelSize;
            tmin = tmax;
            terminate = true;
        } else {
            *dt = (nextCrossingT[stepAxis] - tmin) * voxelSize;
            tmin = nextCrossingT[stepAxis];
        }

        index[0] = voxel[0];
        index[1] = voxel[1];
        index[2] = voxel[2];

        voxel[stepAxis] += step[stepAxis];

        if (voxel[stepAxis] == voxelLimit[stepAxis])
            terminate = true;

        nextCrossingT[stepAxis] += deltaT[stepAxis];
        return true;
    }

public:
    bool terminate = false;
    float voxelSize;
    float tmin, tmax;
    float nextCrossingT[3], deltaT[3];
    int step[3], voxelLimit[3], voxel[3];
};

using GridSampler = nanovdb::SampleFromVoxels<nanovdb::FloatGrid::TreeType, 0, false>;
double HeterogeneousMedium::scaleSample(nanovdb::Vec3R index,
                                        const nanovdb::FloatGrid *grid) const {

    return GridSampler(grid->tree())(index) * sigmaScale;
}

HeterogeneousMedium::HeterogeneousMedium(std::string gridFilePath, std::shared_ptr<PhaseFunction> phase) : Medium(phase) {
    std::string fullGridFilePath = FileUtils::getWorkingDir() + gridFilePath;
    densityGrid = nanovdb::io::readGrid(fullGridFilePath, "density", 1);
    densityFloatGrid = densityGrid.grid<float>();

    if (!densityGrid) {
        std::cout << ".nvdb file must contains density grid\n";
        exit(1);
    }
}

bool HeterogeneousMedium::sampleDistance(MediumSampleRecord *mRec, const Ray &ray, const Intersection &its, Point2d sample) const {

    Point3d origin = ray.origin;
    Vec3d direction = ray.direction;

    auto o_grid = densityFloatGrid->worldToIndexF(nanovdb::Vec3f(origin[0], origin[1], origin[2])),
         d_grid = densityFloatGrid->worldToIndexDirF(nanovdb::Vec3f(direction[0], direction[1], direction[2]));

    origin = Point3d{o_grid[0], o_grid[1], o_grid[2]};
    direction = Vec3d{d_grid[0], d_grid[1], d_grid[2]};

    int index[3];
    float thick = -fm::log(1 - sample[0]);
    float dt, sum = .0f, t_world = .0f;

    RegularTracker rt(minIndex, maxIndex, origin, direction, its.t, voxelSize);

    while (rt.track(index, &dt)) {
        nanovdb::Vec3<double> voxel_loc(index[0] + .5f, index[1] + .5f, index[2] + .5f);
        float density = scaleSample(voxel_loc, densityFloatGrid),
              delta = density * dt;

        if (sum + delta >= thick) {
            // Sample the collision point

            dt = (thick - sum) / density;
            t_world += dt;
            mRec->scatterPoint = ray.at(t_world);
            mRec->marchLength = t_world;
            mRec->sigmaA = Spectrum(.0f);//TODO
            mRec->sigmaS = Spectrum(density);
            mRec->tr = Spectrum(fm::exp(-thick));
            mRec->pdf = mRec->tr[0] * density;//TODO

            return true;
        }
        sum += delta;
        t_world += dt;
    }

    mRec->marchLength = its.t;
    mRec->tr = Spectrum(fm::exp(-sum));
    mRec->pdf = mRec->tr[0];
    return false;
}

Spectrum HeterogeneousMedium::evalTransmittance(Point3d from, Point3d dest) const {
    Point3d origin = from;
    Vec3d direction = normalize(dest - from);
    float t = (dest - from).length();

    auto o_grid = densityFloatGrid->worldToIndexF(nanovdb::Vec3f(origin[0], origin[1], origin[2])),
         d_grid = densityFloatGrid->worldToIndexDirF(nanovdb::Vec3f(direction[0], direction[1], direction[2]));

    origin = Point3d(o_grid[0], o_grid[1], o_grid[2]);
    direction = Vec3d(d_grid[0], d_grid[1], d_grid[2]);

    RegularTracker rt(minIndex, maxIndex, origin, direction, t, voxelSize);

    int index[3];
    float dt, thick = .0f;
    while (rt.track(index, &dt)) {
        nanovdb::Vec3<double> voxel_loc(index[0] + .5f, index[1] + .5f,
                                        index[2] + .5f);
        float density = scaleSample(voxel_loc, densityFloatGrid);
        thick += density * dt;
    }
    return Spectrum(std::exp(-thick));
}