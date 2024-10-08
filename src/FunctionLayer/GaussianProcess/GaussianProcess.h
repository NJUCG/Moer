#pragma once
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Adapter/JsonUtil.h"
#include "FunctionLayer/Sampler/Sampler.h"
#include "Eigen/Dense"

#include "GPFunctions.h"

#include <variant>
enum class MemoryModel {
    None,
    GlobalN,
    Renewal,
    RenewalPlus,
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
struct GaussianProcess {
    GaussianProcess(std::shared_ptr<MeanFunction> _mean, std::shared_ptr<CovarianceFunction> _cov, const GPRealization &_globalCondition);
    virtual GPRealization sample(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir, Sampler &sampler) const;
    virtual GPRealization sampleCond(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir,
                                     const Point3d *pointsCond, const DerivativeType *derivativeTypesCond, const Vec3d *derivativeDirsCond, const double *valuesCond, size_t numPointsCond, const Vec3d &derivativeDirCond,
                                     Sampler &sampler) const;
    std::shared_ptr<MeanFunction> meanFunction;
    std::shared_ptr<CovarianceFunction> covFunction;

    virtual double goodStepSize(Point3d p, Vec3d rd, double desiredCov) const;

protected:
    GPRealization globalCondition;
    std::variant<Eigen::LDLT<Eigen::MatrixXd>, Eigen::BDCSVD<Eigen::MatrixXd, Eigen::ComputeThinU | Eigen::ComputeThinV>> globalCondtionSolver;
    void initGlobalCondition(const GPRealization &_globalCondition);

    // for mean and cov before apply global condition
    Eigen::VectorXd meanPrior(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const;
    Eigen::MatrixXd covPrior(const Point3d *pointsX, const DerivativeType *derivativeTypesX, const Vec3d *derivativeDirsX, size_t numPointsX, const Vec3d &derivativeDirX,
                             const Point3d *pointsY, const DerivativeType *derivativeTypesY, const Vec3d *derivativeDirsY, size_t numPointsY, const Vec3d &derivativeDirY) const;
    Eigen::MatrixXd covPriorSym(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPointsconst, const Vec3d &derivativeDir) const;

    // for mean and cov after apply global condition
    Eigen::VectorXd mean(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const;
    Eigen::MatrixXd cov(const Point3d *pointsX, const DerivativeType *derivativeTypesX, const Vec3d *derivativeDirsX, size_t numPointsX, const Vec3d &derivativeDirX,
                        const Point3d *pointsY, const DerivativeType *derivativeTypesY, const Vec3d *derivativeDirsY, size_t numPointsY, const Vec3d &derivativeDirY) const;
    Eigen::MatrixXd covSym(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPointsconst, const Vec3d &derivativeDir) const;
    std::tuple<Eigen::VectorXd, Eigen::MatrixXd> meanAndCov(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const;
};

#define EXPAND_GPREALIZATION(real) \
    real.points.data(), real.derivativeTypes.data(), real.derivativeDirections.data(), real.size(), {}
#define EXPAND_GPREALIZATION_WITH_VALUE(real) \
    real.points.data(), real.derivativeTypes.data(), real.derivativeDirections.data(), real.values.data(), real.size(), {}