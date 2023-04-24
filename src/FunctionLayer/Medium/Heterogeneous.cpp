#include "Heterogeneous.h"
#include <nanovdb/util/IO.h>
#include <nanovdb/util/SampleFromVoxels.h>
#include <FunctionLayer/Sampler/Independent.h>

using GridSampler =
    nanovdb::SampleFromVoxels<nanovdb::FloatGrid::TreeType, 0, false>;

double HeterogeneousMedium::scaleSample(nanovdb::Vec3R index,
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

    //* Init majorant grid
    int resolution[] = {32, 32, 32};
    majorantGrid = MajorantGrid(resolution, boxMin, boxMax, voxelSize);

    constexpr double delta = 1.;

    auto get_subBox = [](const BoundingBox3f &box, Point3d u_min, Point3d u_max) -> std::pair<Point3d, Point3d> {
        Point3d pMin = box.pMin, pMax = box.pMax;
        double x_min = (1 - u_min[0]) * pMin[0] + u_min[0] * pMax[0],
               y_min = (1 - u_min[1]) * pMin[1] + u_min[1] * pMax[1],
               z_min = (1 - u_min[2]) * pMin[2] + u_min[2] * pMax[2],
               x_max = (1 - u_max[0]) * pMin[0] + u_max[0] * pMax[0],
               y_max = (1 - u_max[1]) * pMin[1] + u_max[1] * pMax[1],
               z_max = (1 - u_max[2]) * pMin[2] + u_max[2] * pMax[2];
        return {Point3d{x_min, y_min, z_min}, Point3d{x_max, y_max, z_max}};
    };

    //* Set majorant at each majorantVoxel
    for (int iz = 0; iz < resolution[2]; ++iz) {
        for (int iy = 0; iy < resolution[1]; ++iy) {
            for (int ix = 0; ix < resolution[0]; ++ix) {
                Point3d u_min, u_max;
                u_min[0] = (double)ix / resolution[0],
                u_min[1] = (double)iy / resolution[1];
                u_min[2] = (double)iz / resolution[2];

                u_max[0] = (double)(ix + 1) / resolution[0],
                u_max[1] = (double)(iy + 1) / resolution[1];
                u_max[2] = (double)(iz + 1) / resolution[2];

                auto [vmin, vmax] = get_subBox(majorantGrid.box, u_min, u_max);
                nanovdb::Vec3<double> voxelMin = densityFloatGrid->worldToIndexF(
                    nanovdb::Vec3<double>(vmin[0], vmin[1], vmin[2]));
                nanovdb::Vec3<double> voxelMax = densityFloatGrid->worldToIndexF(
                    nanovdb::Vec3<double>(vmax[0], vmax[1], vmax[2]));

                double maj = .0;
                for (int nz = voxelMin[2] - delta; nz <= voxelMax[2] + delta; ++nz)
                    for (int ny = voxelMin[1] - delta; ny <= voxelMax[1] + delta; ++ny)
                        for (int nx = voxelMin[0] - delta; nx <= voxelMax[0] + delta; ++nx)
                            maj = std::max(maj, scaleSample(nanovdb::Vec3R(nx, ny, nz),
                                                            densityFloatGrid));
                majorantGrid.set(ix, iy, iz, maj);
            }
        }
    }
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

bool HeterogeneousMedium::sampleDistanceDeltaTracking(MediumSampleRecord *mRec,
                                                      const Ray &ray,
                                                      const Intersection &its,
                                                      Point2d sample) const {
    auto getUniformCoord = [](const BoundingBox3f &box, Point3d p) {
        Point3d uCoord;
        for (int axis = 0; axis < 3; ++axis) {
            uCoord[axis] = (p[axis] - box.pMin[axis]) / (box.pMax[axis] - box.pMin[axis]);
        }
        return uCoord;
    };

    Point3d uCoord = getUniformCoord(majorantGrid.box, ray.origin);
    MajorantTracker mt = majorantGrid.getTracker(uCoord, ray.direction, its.t);

    double thick = -std::log(1 - sample[0]);

    int index[3];
    float dt, sum = .0f;
    float t_world = .0f;

    // TODO
    mRec->tr = Spectrum(1.0);
    mRec->pdf = 1.0;

    while (mt.track(index, &dt)) {
        float maj = majorantGrid.at(index[0], index[1], index[2]), delta = maj * dt;

        if (sum + delta >= thick) {
            dt = (thick - sum) / maj;
            t_world += dt;

            mRec->scatterPoint = ray.at(t_world);
            mRec->marchLength = t_world;

            nanovdb::Vec3<double> indexLoc =
                densityFloatGrid->worldToIndexF(nanovdb::Vec3<double>(
                    mRec->scatterPoint[0],
                    mRec->scatterPoint[1],
                    mRec->scatterPoint[2]));
            float density = scaleSample(indexLoc, densityFloatGrid);

            mRec->sigmaMaj = maj;
            mRec->sigmaS = density;// TODO albedo
            mRec->sigmaA = Spectrum(.0);

            return true;
        }

        t_world += dt;
        sum += delta;
    }
    return false;
}

Spectrum HeterogeneousMedium::evalTransmittanceRatioTracking(Point3d from, Point3d dest) const {
    static IndependentSampler sampler;

    auto getUniformCoord = [](const BoundingBox3f &box, Point3d p) {
        Point3d uCoord;
        for (int axis = 0; axis < 3; ++axis) {
            uCoord[axis] = (p[axis] - box.pMin[axis]) / (box.pMax[axis] - box.pMin[axis]);
        }
        return uCoord;
    };

    Spectrum Tr(1.0);

    Point3d uCoord = getUniformCoord(majorantGrid.box, from);
    Vec3d dir = normalize(dest - from);
    double distance = (dest - from).length();
    MajorantTracker mt = majorantGrid.getTracker(uCoord, dir, distance);

    int index[3], axis;
    float dt;
    double thick_sum = .0, t_sum = .0;
    float thick = -std::log(1 - sampler.sample1D());

    while (mt.track(index, &dt, &axis)) {
        float maj = majorantGrid.at(index[0], index[1], index[2]), delta = dt * maj;

        if (thick_sum + delta >= thick) {
            float step = (thick - thick_sum) / maj;
            t_sum += step;
            Point3d position = from + t_sum * dir;
            nanovdb::Vec3<double> indexLoc = densityFloatGrid->worldToIndexF(
                nanovdb::Vec3<double>(position[0], position[1], position[2]));
            float sigma_t = scaleSample(indexLoc, densityFloatGrid);
            Tr *= Spectrum(1.f) - sigma_t / maj;

            // Update
            thick_sum = .0;
            thick = -std::log(1 - sampler.sample1D());
            mt.terminate = false;
            mt.voxel[0] = index[0];
            mt.voxel[1] = index[1];
            mt.voxel[2] = index[2];
            mt.tmin = mt.tmin - dt + step;
            mt.nextCrossingT[axis] -= mt.deltaT[axis];
            continue;
        }

        t_sum += dt;
        thick_sum += delta;
    }
}