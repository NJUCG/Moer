#include "Heterogeneous.h"
#include <nanovdb/util/IO.h>
#include <nanovdb/util/SampleFromVoxels.h>

using GridSampler =
    nanovdb::SampleFromVoxels<nanovdb::FloatGrid::TreeType, 0, false>;

float HeterogeneousMedium::scaleSample(nanovdb::Vec3R index,
                                       const nanovdb::FloatGrid *grid) const {

    return GridSampler(grid->tree())(index) * sigmaScale;
}

class RegularTracker {
public:
    RegularTracker() = delete;

    RegularTracker(const int min[3], const int max[3], Point3d origin,
                   Vec3d direction, float _tmax, float _voxelSize) {
        voxelSize = _voxelSize;
        tmin = .0f;
        tmax = _tmax / voxelSize;

        for (int axis = 0; axis < 3; ++axis) {
            voxel[axis] = clamp((int)std::floor(origin[axis]), min[axis], max[axis]);

            deltaT[axis] = 1.f / std::abs(direction[axis]);

            if (direction[axis] == -.0f)
                direction[axis] = .0f;

            if (direction[axis] >= 0) {
                nextCrossingT[axis] =
                    (voxel[axis] + 1.f - origin[axis]) / direction[axis];
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
        if (terminate)
            return false;

        int stepAxis = -1;

        //* If x hit first
        if (nextCrossingT[0] < nextCrossingT[1] &&
            nextCrossingT[0] < nextCrossingT[2])
            stepAxis = 0;
        //* If y hit first
        else if (nextCrossingT[1] < nextCrossingT[2])
            stepAxis = 1;
        else
            stepAxis = 2;

        if (nextCrossingT[stepAxis] > tmax) {
            *dt = (tmax - tmin) * voxelSize;
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

        if (voxel[stepAxis] == voxelLimit[stepAxis]) {
            terminate = true;
        }

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

bool MajorantTracker::track(int *index, float *dt, int *axis) {

    if (terminate)
        return false;

    int stepAxis = -1;

    //* If x hit first
    if (nextCrossingT[0] < nextCrossingT[1] &&
        nextCrossingT[0] < nextCrossingT[2])
        stepAxis = 0;
    //* If y hit first
    else if (nextCrossingT[1] < nextCrossingT[2])
        stepAxis = 1;
    else
        stepAxis = 2;

    if (axis)
        *axis = stepAxis;

    if (nextCrossingT[stepAxis] > tmax) {
        *dt = tmax - tmin;
        tmin = tmax;
        terminate = true;
    } else {
        *dt = nextCrossingT[stepAxis] - tmin;
        tmin = nextCrossingT[stepAxis];
    }

    index[0] = voxel[0];
    index[1] = voxel[1];
    index[2] = voxel[2];

    voxel[stepAxis] += step[stepAxis];

    if (voxel[stepAxis] == voxelLimit[stepAxis]) {
        terminate = true;
    }

    nextCrossingT[stepAxis] += deltaT[stepAxis];
    return true;
}

MajorantTracker MajorantGrid::getTracker(Point3d origin_u, Vec3d dir_u,
                                         float t_world) const {
    int index[3], step[3], voxelLimit[3];
    float deltaT[3];       // World Space
    float nextCrossingT[3];// World Space
    MajorantTracker mt;

    mt.tmin = .0f;
    mt.tmax = t_world;

    for (int axis = 0; axis < 3; ++axis) {
        index[axis] =
            clamp(origin_u[axis] * resulotion[axis], 0, resulotion[axis] - 1);
        deltaT[axis] = ((box.pMax[axis] - box.pMin[axis]) / resulotion[axis]) /
                       std::abs(dir_u[axis]);
        if (dir_u[axis] == -.0f)
            dir_u[axis] = .0f;

        if (dir_u[axis] >= 0) {
            nextCrossingT[axis] =
                ((((float)index[axis] + 1.f) / resulotion[axis]) - origin_u[axis]) *
                (box.pMax[axis] - box.pMin[axis]) / dir_u[axis];
            step[axis] = 1;
            voxelLimit[axis] = resulotion[axis];
        } else {
            nextCrossingT[axis] =
                (((float)index[axis] / resulotion[axis]) - origin_u[axis]) *
                (box.pMax[axis] - box.pMin[axis]) / dir_u[axis];
            step[axis] = -1;
            voxelLimit[axis] = -1;
        }

        mt.voxel[axis] = index[axis];
        mt.deltaT[axis] = deltaT[axis];
        mt.nextCrossingT[axis] = nextCrossingT[axis];
        mt.step[axis] = step[axis];
        mt.voxelLimit[axis] = voxelLimit[axis];
    }

    return mt;
}

HeterogeneousMedium::HeterogeneousMedium(std::shared_ptr<PhaseFunction> phase) : Medium(phase) {
    //TODO hard code
    std::string vdbfilepath = "/home/zcx/Programming/Moer/scenes/heterogeneous/models/cloud.nvdb";
    densityGrid = nanovdb::io::readGrid(vdbfilepath, "density", 1);
    densityFloatGrid = densityGrid.grid<float>();
    if (!densityGrid) {
        std::cout << ".nvdb file must contains density grid\n";
        exit(1);
    }
    // Assume every voxel is a cube
    voxelSize = densityFloatGrid->voxelSize()[0];

    temperatureGrid = nanovdb::io::readGrid(vdbfilepath, "temperature", 1);
    temperatureFloatGrid = temperatureGrid.grid<float>();

    //* Compute density grid bound
    auto bbox = densityFloatGrid->worldBBox();
    boxMin = Point3d(bbox.min()[0], bbox.min()[1], bbox.min()[2]);
    boxMax = Point3d(bbox.max()[0], bbox.max()[1], bbox.max()[2]);

    minIndex[0] = densityFloatGrid->indexBBox().min().x();
    minIndex[1] = densityFloatGrid->indexBBox().min().y();
    minIndex[2] = densityFloatGrid->indexBBox().min().z();

    maxIndex[0] = densityFloatGrid->indexBBox().max().x();
    maxIndex[1] = densityFloatGrid->indexBBox().max().y();
    maxIndex[2] = densityFloatGrid->indexBBox().max().z();

    sigmaScale = 1.f;// Default
}

bool HeterogeneousMedium::sampleDistance(MediumSampleRecord *mRec, const Ray &ray, const Intersection &its, Point2d sample) const {
    Point3d o = ray.origin;
    Vec3d d = ray.direction;

    auto o_grid =
             densityFloatGrid->worldToIndexF(nanovdb::Vec3f(o[0], o[1], o[2])),
         d_gird =
             densityFloatGrid->worldToIndexDirF(nanovdb::Vec3f(d[0], d[1], d[2]));

    o = Point3d{o_grid[0], o_grid[1], o_grid[2]};
    d = normalize(Vec3d{d_gird[0], d_gird[1], d_gird[2]});

    // * thick = \sum sigma_t * step
    float thick = -std::log(1 - sample[0]);

    int index[3];
    float dt, sum = .0f;
    float t_world = .0f;

    //* Init regularTracker (all params in indexSpace)
    RegularTracker rt(minIndex, maxIndex, o, d, its.t, voxelSize);

    int step = 0;
    // dt in worldSpace
    while (rt.track(index, &dt)) {
        nanovdb::Vec3<double> voxel_loc(index[0] + .5f, index[1] + .5f,
                                        index[2] + .5f);

        float density = scaleSample(voxel_loc, densityFloatGrid),
              delta = density * dt;

        if (sum + delta >= thick) {

            //* Sample a valid point before exit the medium
            dt = (thick - sum) / density;

            t_world += dt;
            mRec->scatterPoint = ray.at(t_world);
            mRec->marchLength = t_world;
            mRec->sigmaA = Spectrum(.0f);
            mRec->sigmaS = Spectrum(density);
            mRec->tr = Spectrum(std::exp(-thick));
            mRec->pdf = mRec->tr[0] * density;// TODO

            return true;
        }

        sum += delta;
        t_world += dt;
    }

    mRec->marchLength = its.t;
    mRec->tr = Spectrum(std::exp(-sum));
    mRec->pdf = (mRec->tr)[0];

    return false;
}

Spectrum HeterogeneousMedium::evalTransmittance(Point3d from, Point3d dest) const {
    Point3d o = from;
    Vec3d d = normalize(dest - from);
    float t = (dest - from).length();

    auto o_grid =
             densityFloatGrid->worldToIndexF(nanovdb::Vec3f(o[0], o[1], o[2])),
         d_gird =
             densityFloatGrid->worldToIndexDirF(nanovdb::Vec3f(d[0], d[1], d[2]));

    o = Point3d{o_grid[0], o_grid[1], o_grid[2]};
    d = normalize(Vec3d{d_gird[0], d_gird[1], d_gird[2]});

    RegularTracker rt(minIndex, maxIndex, o, d, t, voxelSize);
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