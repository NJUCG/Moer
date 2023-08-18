#include "Heterogeneous.h"
#include <nanovdb/util/IO.h>
#include <nanovdb/util/SampleFromVoxels.h>
#include <FunctionLayer/Sampler/Independent.h>
#include <ResourceLayer/File/FileUtils.h>

class RegularTracker {
public:
    RegularTracker() = delete;

    RegularTracker(const int min[3], const int max[3], Point3d origin, Vec3d direction, float _tmax, float _voxelSize, float *t_world = nullptr) {
        voxelSize = _voxelSize * 0.03;
        tmin = .0f;
        tmax = _tmax / voxelSize;

        {
            // Check if the ray overlap the voxelGrid
            float minBound = -FLT_MAX, maxBound = FLT_MAX;
            for (int axis = 0; axis < 3; ++axis) {
                float t1 = (min[axis] - origin[axis]) / direction[axis],
                      t2 = (max[axis] - origin[axis]) / direction[axis];
                if (t1 > t2) std::swap(t1, t2);
                minBound = std::max(minBound, t1);
                maxBound = std::min(maxBound, t2);

                if ((minBound > maxBound) || maxBound < 0) terminate = true;
            }

            if (!terminate) {
                // Set the ray origin at the bound of the grid
                float t = minBound > 0 ? minBound : 0;
                origin += direction * t;
                tmax -= t;
                if (t_world) *t_world = t * voxelSize;
            }
        }

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
            *dt = (tmax - tmin) * voxelSize;
            tmin = tmax;
            terminate = true;
            //     std::cout << "True";
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

Point3d HeterogeneousMedium::worldToIndex(Point3d world) const {
    world = invTransformMatrix * world;
    auto index = densityFloatGrid->worldToIndexF(nanovdb::Vec3f{(float)world[0], (float)world[1], (float)world[2]});
    return Point3d{index[0], index[1], index[2]};
}

Point3d HeterogeneousMedium::indexToWorld(Point3d index) const {
    nanovdb::Vec3f idx{(float)index[0], (float)index[1], (float)index[2]};
    auto wrd = densityFloatGrid->indexToWorldF(idx);
    Point3d world{wrd[0], wrd[1], wrd[2]};
    return transformMatrix * world;
}

Vec3d HeterogeneousMedium::worldToIndexDir(Vec3d world) const {
    world = invTransformMatrix * world;
    auto index = densityFloatGrid->worldToIndexDirF(nanovdb::Vec3f{(float)world[0], (float)world[1], (float)world[2]});
    return normalize(Vec3d{index[0], index[1], index[2]});
}

Vec3d HeterogeneousMedium::indexToWorldDir(Vec3d index) const {
    nanovdb::Vec3f idx{(float)index[0], (float)index[1], (float)index[2]};
    auto wrd = densityFloatGrid->indexToWorldDirF(idx);
    Vec3d world{wrd[0], wrd[1], wrd[2]};
    return normalize(transformMatrix * world);
}

HeterogeneousMedium::HeterogeneousMedium(std::string gridFilePath, std::shared_ptr<PhaseFunction> phase, TransformMatrix3D _transformMatrix, float _sigmaScale) : Medium(phase) {
    transformMatrix = _transformMatrix;
    invTransformMatrix = _transformMatrix.getInverse();

    std::string fullGridFilePath = FileUtils::getWorkingDir() + gridFilePath;
    densityGrid = nanovdb::io::readGrid(fullGridFilePath, "density", 1);
    densityFloatGrid = densityGrid.grid<float>();

    sigmaScale = _sigmaScale;

    if (!densityGrid) {
        std::cout << ".nvdb file must contains density grid\n";
        exit(1);
    }

    // Assume every voxel is cube
    voxelSize = densityFloatGrid->voxelSize()[0];

    auto bbox = densityFloatGrid->worldBBox();
    Point3d boxMin = Point3d(bbox.min()[0], bbox.min()[1], bbox.min()[2]);
    Point3d boxMax = Point3d(bbox.max()[0], bbox.max()[1], bbox.max()[2]);

    //* Compute density grid bound
    minIndex[0] = densityFloatGrid->indexBBox().min().x();
    minIndex[1] = densityFloatGrid->indexBBox().min().y();
    minIndex[2] = densityFloatGrid->indexBBox().min().z();

    maxIndex[0] = densityFloatGrid->indexBBox().max().x();
    maxIndex[1] = densityFloatGrid->indexBBox().max().y();
    maxIndex[2] = densityFloatGrid->indexBBox().max().z();
}

bool HeterogeneousMedium::sampleDistance(MediumSampleRecord *mRec, const Ray &ray, const Intersection &its, Point2d sample) const {

    //    nanovdb::Vec3f world_coordinate{5.62f, 17.6f, 3.221f};
    //    auto index_coordinate = densityFloatGrid->worldToIndexF(world_coordinate);
    //    printf("%.4f, %.4f, %.4f\n", index_coordinate[0], index_coordinate[1], index_coordinate[2]);
    //
    //    Point3d world_p{5.62f, 17.6f, 3.221f};
    //    auto inverse = transformMatrix.getInverse();
    //    Point3d p_prim = inverse * (world_p);
    //
    //    nanovdb::Vec3f world_p_prim{(float)p_prim[0], (float)p_prim[1], (float)p_prim[2]};
    //    auto index_p_prim = densityFloatGrid->worldToIndexF(world_p_prim);
    //    printf("%.4f, %.4f, %.4f\n", index_p_prim[0], index_p_prim[1], index_p_prim[2]);
    //
    //    auto world_p_prim_ = densityFloatGrid->indexToWorld(index_p_prim);
    //    Point3d res = transformMatrix * Point3d((float)world_p_prim_[0], (float)world_p_prim_[1], (float)world_p_prim_[2]);
    //    printf("%.4f, %.4f, %.4f\n", res[0], res[1], res[2]);
    //    exit(1);

    ////Point3d origin = ray.origin;
    ////Vec3d direction = ray.direction;
    ////
    ////auto o_grid = densityFloatGrid->worldToIndexF(nanovdb::Vec3f(origin[0], origin[1], origin[2])),
    ////     d_grid = densityFloatGrid->worldToIndexDirF(nanovdb::Vec3f(direction[0], direction[1], direction[2]));
    ////
    ////origin = Point3d{o_grid[0], o_grid[1], o_grid[2]};
    ////direction = normalize(Vec3d{d_grid[0], d_grid[1], d_grid[2]});

    Point3d origin = worldToIndex(ray.origin);
    Vec3d direction = worldToIndexDir(ray.direction);

    int index[3];
    float thick = -fm::log(1 - sample[0]);
    float dt, sum = .0f, t_world = .0f;

    RegularTracker rt(minIndex, maxIndex, origin, direction, its.t, voxelSize, &t_world);

    while (rt.track(index, &dt)) {
        nanovdb::Vec3<double> voxel_loc(index[0] + .5f, index[1] + .5f, index[2] + .5f);
        float density = scaleSample(voxel_loc, densityFloatGrid),
              delta = density * dt;

        if (sum + delta >= thick) {
            // Sample the collision point

            dt = (thick - sum) / density;
            t_world += dt;
            mRec->scatterPoint = ray.at(t_world);

            Point3d idx = worldToIndex(mRec->scatterPoint);

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
    ////Point3d origin = from;
    ////Vec3d direction = normalize(dest - from);
    ////
    ////auto o_grid = densityFloatGrid->worldToIndexF(nanovdb::Vec3f(origin[0], origin[1], origin[2])),
    ////     d_grid = densityFloatGrid->worldToIndexDirF(nanovdb::Vec3f(direction[0], direction[1], direction[2]));
    ////
    ////origin = Point3d{o_grid[0], o_grid[1], o_grid[2]};
    ////direction = normalize(Vec3d(d_grid[0], d_grid[1], d_grid[2]));

    Vec3d direction = normalize(dest - from);
    float t = (dest - from).length();
    Point3d origin = worldToIndex(from);

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