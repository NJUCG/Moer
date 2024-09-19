#pragma once
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Adapter/JsonUtil.h"
#include "FunctionLayer/Sampler/Sampler.h"
#include "Eigen/Dense"

#include "GPFunctions.h"
enum class DerivativeType {
    None,
    First,
    Second,
};
enum class MemoryModel {
    None,
    GlobalN,
    Renewal,
    RenewalPlus,
};

struct GaussianProcess;
struct GPRealization {
    GPRealization() : gp(nullptr){};
    GPRealization(const GaussianProcess* _gp, const Point3d *_points, const DerivativeType *_derivativeTypes, const Vec3d *_derivativeDirs, const double *_values, size_t numPoints, const Vec3d &derivativeDir);

    std::vector<Point3d> points;
    std::vector<DerivativeType> derivativeTypes;
    std::vector<Vec3d> derivativeDirections;
    std::vector<double> values;

    // there is a zero-crossing between values[p-1] ~ values[p]
    virtual void makeIntersection(size_t p);
    virtual Vec3d sampleGradient(Point3d pos, Vec3d rayDir, Sampler &sampler);
    virtual void applyMemoryModel(Vec3d rayDir, MemoryModel memoryModel = MemoryModel::None);

    bool isEmpty() const { return points.empty(); }

    bool justIntersected = false;
    Vec3d lastSampledGrad;
    const GaussianProcess* gp;
};
struct GaussianProcess {
    GaussianProcess(std::shared_ptr<MeanFunction> _mean, std::shared_ptr<CovarianceFunction> _cov);
    GaussianProcess(const Json &json);
    virtual GPRealization sample(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir, Sampler &sampler) const;
    virtual GPRealization sampleCond(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir,
                                     const Point3d *pointsCond, const DerivativeType *derivativeTypesCond, const Vec3d *derivativeDirsCond, const double* valuesCond,size_t numPointsCond, const Vec3d &derivativeDirCond,
                                     Sampler &sampler) const;

    std::shared_ptr<MeanFunction> meanFunction;
    std::shared_ptr<CovarianceFunction> covFunction;

    // TODO(Cchen77): Global Conditioning
    GPRealization globalCondition;

protected:
    // for mean and cov before apply global condition
    Eigen::VectorXd meanPrior(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const;
    Eigen::MatrixXd covPrior(const Point3d *pointsX, const DerivativeType *derivativeTypesX, const Vec3d *derivativeDirsX, const Vec3d &derivativeDirX,size_t numPointsX,
                             const Point3d *pointsY, const DerivativeType *derivativeTypesY, const Vec3d *derivativeDirsY, const Vec3d &derivativeDirY,size_t numPointsY) const;
    Eigen::MatrixXd covPriorSym(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, const Vec3d &derivativeDir, size_t numPoints) const;

    // for mean and cov after apply global condition
    Eigen::VectorXd mean(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const;
    Eigen::MatrixXd cov(const Point3d *pointsX, const DerivativeType *derivativeTypesX, const Vec3d *derivativeDirsX, const Vec3d &derivativeDirX, size_t numPointsX,
                        const Point3d *pointsY, const DerivativeType *derivativeTypesY, const Vec3d *derivativeDirsY, const Vec3d &derivativeDirY, size_t numPointsY) const;
    Eigen::MatrixXd covSym(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, const Vec3d &derivativeDir, size_t numPoint) const;
    std::tuple<Eigen::VectorXd, Eigen::MatrixXd> meanAndCov(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const;
};
