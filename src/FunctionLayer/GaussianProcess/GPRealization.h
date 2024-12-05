#pragma once
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Adapter/JsonUtil.h"
#include "FunctionLayer/Sampler/Sampler.h"
#include "Eigen/Dense"

#include "GaussianProcessUtils.h"
enum class MemoryModel {
    None,
    GlobalN,
    Renewal,
    RenewalPlus,
    Clear,
};

struct GaussianProcess;
struct GPRealization {
    GPRealization() : gp(nullptr){};
    GPRealization(const GaussianProcess *_gp, const Point3d *_points, const DerivativeType *_derivativeTypes, const Vec3d *_derivativeDirs, const double *_values, size_t numPoints, const Vec3d &derivativeDir);

    std::vector<Point3d> points;
    std::vector<DerivativeType> derivativeTypes;
    std::vector<Vec3d> derivativeDirections;
    std::vector<double> values;

    // there is a zero-crossing between values[p-1] ~ values[p]
    virtual void makeIntersection(size_t p, double offset);
    virtual void manualIntersectionAndNormal(Point3d pos, Vec3d rayDir, double grad);

    virtual Vec3d sampleGradient(Point3d pos, Vec3d rayDir, Sampler &sampler);
    virtual void applyMemoryModel(Vec3d rayDir, MemoryModel memoryModel = MemoryModel::None);

    size_t size() const { return points.size(); }
    bool isEmpty() const { return points.empty(); }

    bool justIntersected = false;
    Vec3d lastSampledGrad;
    const GaussianProcess *gp;

    void reset() {
        points.clear();
        derivativeTypes.clear();
        derivativeDirections.clear();
        values.clear();

        justIntersected = false;
        lastSampledGrad = {};
        gp = nullptr;
    }
};

#define EXPAND_GPREALIZATION(real) \
    real.points.data(), real.derivativeTypes.data(), real.derivativeDirections.data(), real.size(), {}
#define EXPAND_GPREALIZATION_WITH_VALUE(real) \
    real.points.data(), real.derivativeTypes.data(), real.derivativeDirections.data(), real.values.data(), real.size(), {}
