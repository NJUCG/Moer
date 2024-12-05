#pragma once
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Adapter/JsonUtil.h"
#include "FunctionLayer/Sampler/Sampler.h"
#include "CoreLayer/Ray/Ray.h"
#include "Eigen/Dense"

#include "GaussianProcessUtils.h"
#include "GPFunctions.h"
#include "GPRealization.h"

#include <variant>
struct GaussianProcess {
    GaussianProcess(std::shared_ptr<MeanFunction> _mean, std::shared_ptr<CovarianceFunction> _cov, const GPRealization &_globalCondition);
    virtual GPRealization sample(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir, Sampler &sampler) const;
    virtual GPRealization sampleCond(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir,
                                     const Point3d *pointsCond, const DerivativeType *derivativeTypesCond, const Vec3d *derivativeDirsCond, const double *valuesCond, size_t numPointsCond, const Vec3d &derivativeDirCond,
                                     Sampler &sampler) const;

    virtual double sampleFPT(const Ray &ray, double &t, double numSampleCount, Sampler &sampler);
    virtual double sampleFPTCond(const Ray &ray, double &t, double numSampleCount, Sampler &sampler,
                               const Point3d *pointsCond, const DerivativeType *derivativeTypesCond, const Vec3d *derivativeDirsCond, const double *valuesCond, size_t numPointsCond, const Vec3d &derivativeDirCond);

    std::shared_ptr<MeanFunction> meanFunction;
    std::shared_ptr<CovarianceFunction> covFunction;

    // return a heuristics stepsize base on meanFunction and covFunction
    virtual double goodStepSize(Point3d p, Vec3d rd, double desiredCov, double stepSize) const;

    double meanZeroDownCrossingRate(const Point3d &pos, const Vec3d &ddir);

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
