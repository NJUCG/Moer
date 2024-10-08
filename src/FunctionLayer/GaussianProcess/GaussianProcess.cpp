#include "GaussianProcess.h"

#include "GaussianProcessUtils.h"
#include "CoreLayer/Geometry/Frame.h"

GPRealization::GPRealization(const GaussianProcess *_gp,
                             const Point3d *_points,
                             const DerivativeType *_derivativeTypes,
                             const Vec3d *_derivativeDirs,
                             const double *_values,
                             size_t numPoints, const Vec3d &derivativeDir) : gp(_gp) {
    for (int i = 0; i < numPoints; ++i) {
        Vec3d ddir = _derivativeDirs ? _derivativeDirs[i] : derivativeDir;
        points.push_back(_points[i]);
        derivativeTypes.push_back(_derivativeTypes[i]);
        derivativeDirections.push_back(ddir);
        values.push_back(_values[i]);
    }
}

void GPRealization::makeIntersection(size_t p, double offset) {
    auto preV = values[p - 1];
    auto curV = values[p];

    Point3d zeroCrossing = lerp(points[p - 1], points[p], offset);
    double gradValue = (curV - preV) / ((points[p] - points[p - 1]).length());

    points.push_back(zeroCrossing);
    derivativeTypes.push_back(DerivativeType::None);
    values.push_back(lerp(preV, curV, offset));
    // place holder
    derivativeDirections.push_back({});

    points.push_back(zeroCrossing);
    derivativeTypes.push_back(DerivativeType::First);
    values.push_back(gradValue);
    derivativeDirections.push_back(normalize(points[p] - points[p - 1]));

    justIntersected = true;
}

Vec3d GPRealization::sampleGradient(Point3d pos, Vec3d rayDir, Sampler &sampler) {
    std::array<Point3d, 3> gradPs{pos, pos, pos};
    std::array<DerivativeType, 3> gradDerivs{DerivativeType::First, DerivativeType::First, DerivativeType::First};

    Frame frame(rayDir);

    Vec3d sampleGrad = {};

    // we just apply 'makeIntersect' method
    if (justIntersected) {
        std::array<Vec3d, 2> gradDirs{
            vec_conv<Vec3d>(frame.s),
            vec_conv<Vec3d>(frame.t)};
        auto realization = gp->sampleCond(gradPs.data(), gradDerivs.data(), gradDirs.data(), gradDirs.size(), {},
                                          points.data(), derivativeTypes.data(), derivativeDirections.data(), values.data(), points.size(), {}, sampler);
        // intersection's gradient is already known since we perform linear interpolation between points
        sampleGrad = frame.toWorld({realization.values[0], realization.values[1], values[values.size() - 1]});

    } else {
        std::array<Vec3d, 3> gradDirs{
            vec_conv<Vec3d>(frame.s),
            vec_conv<Vec3d>(frame.t),
            vec_conv<Vec3d>(frame.n)};
        auto realization = gp->sampleCond(gradPs.data(), gradDerivs.data(), gradDirs.data(), gradDirs.size(), {},
                                          points.data(), derivativeTypes.data(), derivativeDirections.data(), values.data(), points.size(), {}, sampler);
        sampleGrad = frame.toWorld({realization.values[0], realization.values[1], realization.values[2]});
    }
    return lastSampledGrad = sampleGrad;
}

void GPRealization::applyMemoryModel(Vec3d rayDir, MemoryModel memoryModel) {
    std::vector<Point3d> pointsNew;
    std::vector<Vec3d> derivativeDirectionsNew;
    std::vector<DerivativeType> derivativeTypesNew;
    std::vector<double> valuesNew;

    size_t pointSize = points.size();
    switch (memoryModel) {
        case MemoryModel::None:
            break;
        case MemoryModel::GlobalN:
            // TODO(Cchen77): GlobalN memory model
            break;
        case MemoryModel::Renewal: {
            size_t p = pointSize - 1;
            if (justIntersected) {
                p = pointSize - 2;
            }
            pointsNew.push_back(points[p]);
            derivativeDirectionsNew.push_back(derivativeDirections[p]);
            derivativeTypesNew.push_back(derivativeTypes[p]);
            valuesNew.push_back(values[p]);

            points = pointsNew;
            derivativeDirections = derivativeDirectionsNew;
            derivativeTypes = derivativeTypesNew;
            values = valuesNew;
            break;
        }
        case MemoryModel::RenewalPlus: {
            size_t p = pointSize - 1;
            if (justIntersected) {
                p = pointSize - 2;
            }
            pointsNew.push_back(points[p]);
            derivativeDirectionsNew.push_back({});
            derivativeTypesNew.push_back(DerivativeType::None);
            valuesNew.push_back(values[p]);

            pointsNew.push_back(points[p]);
            derivativeDirectionsNew.push_back(rayDir);
            derivativeTypesNew.push_back(DerivativeType::First);
            valuesNew.push_back(dot(lastSampledGrad, rayDir));

            points = pointsNew;
            derivativeDirections = derivativeDirectionsNew;
            derivativeTypes = derivativeTypesNew;
            values = valuesNew;
            break;
        }
        default:
            break;
    }
}

GaussianProcess::GaussianProcess(std::shared_ptr<MeanFunction> _mean, std::shared_ptr<CovarianceFunction> _cov, const GPRealization &_globalCondition) : meanFunction(_mean), covFunction(_cov) {
    initGlobalCondition(_globalCondition);
}

void GaussianProcess::initGlobalCondition(const GPRealization &_globalCondition) {
    globalCondition = _globalCondition;

    // solver picking strategy from https://github.com/daseyb/gpis-light-transport/blob/main/src/core/math/GaussianProcess.cpp
    bool succesfullSolve = false;
    auto kCC = covPriorSym(EXPAND_GPREALIZATION(globalCondition));
    if (kCC.rows() <= 64) {
        Eigen::LDLT<Eigen::MatrixXd> solver(kCC.triangularView<Eigen::Lower>());
        if (solver.info() == Eigen::ComputationInfo::Success && solver.isPositive()) {
            globalCondtionSolver = solver;
            succesfullSolve = true;
        }
    }
    if (!succesfullSolve) {
        Eigen::BDCSVD<Eigen::MatrixXd, Eigen::ComputeThinU | Eigen::ComputeThinV> solver;
        solver.compute(kCC.triangularView<Eigen::Lower>());
        if (solver.info() != Eigen::ComputationInfo::Success) {
            globalCondtionSolver = solver;
            succesfullSolve = true;
        }
    }
    if (!succesfullSolve) {
        std::cerr << "Fail to find a suitable solver for global condition!\n";
    }
}

GPRealization GaussianProcess::sample(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir, Sampler &sampler) const {
    std::vector<double> values;
    auto [_mean, _cov] = meanAndCov(points, derivativeTypes, derivativeDirs, numPoints, derivativeDir);
    MultiVariableNormalDistribution mvn(_mean, _cov);
    auto v = mvn.sample(sampler);
    for (int i = 0; i < numPoints; ++i) {
        values.push_back(v(i));
    }

    return GPRealization(this, points, derivativeTypes, derivativeDirs, values.data(), numPoints, derivativeDir);
}

GPRealization GaussianProcess::sampleCond(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir,
                                          const Point3d *pointsCond, const DerivativeType *derivativeTypesCond, const Vec3d *derivativeDirsCond, const double *valuesCond, size_t numPointsCond, const Vec3d &derivativeDirCond,
                                          Sampler &sampler) const {
    if (numPointsCond == 0) {
        return sample(points, derivativeTypes, derivativeDirs, numPoints, derivativeDir, sampler);
    }

    Eigen::MatrixXd kCC = covSym(pointsCond, derivativeTypesCond, derivativeDirsCond, numPointsCond, derivativeDirCond);
    Eigen::MatrixXd kCx = cov(pointsCond, derivativeTypesCond, derivativeDirsCond, numPointsCond, derivativeDirCond,
                              points, derivativeTypes, derivativeDirs, numPoints, derivativeDir);
    // we should solve kCC*x = kCx => xT kCC = kxC => xT = kxC*kCC^-1
    // solving strategy from https://github.com/daseyb/gpis-light-transport/blob/main/src/core/math/GaussianProcess.cpp
    Eigen::MatrixXd solved;
    bool succesfullSolve = false;
    if (kCC.rows() <= 64) {
        Eigen::LDLT<Eigen::MatrixXd> solver(kCC.triangularView<Eigen::Lower>());
        if (solver.info() == Eigen::ComputationInfo::Success && solver.isPositive()) {
            solved = solver.solve(kCx).transpose();
            if (solver.info() == Eigen::ComputationInfo::Success) {
                succesfullSolve = true;
            } else {
                std::cerr << "Conditioning solving failed (LDLT)!\n";
            }
        }
    }
    if (!succesfullSolve) {
        Eigen::BDCSVD<Eigen::MatrixXd, Eigen::ComputeThinU | Eigen::ComputeThinV> solver;
        solver.compute(kCC.triangularView<Eigen::Lower>());

        if (solver.info() != Eigen::ComputationInfo::Success) {
            std::cerr << "Conditioning decomposition failed (BDCSVD)!\n";
        }

        solved = solver.solve(kCx).transpose();
        if (solver.info() != Eigen::ComputationInfo::Success) {
            std::cerr << "Conditioning solving failed (BDCSVD)!\n";
        }
    }

    auto [meanxx, kxx] = meanAndCov(points, derivativeTypes, derivativeDirs, numPoints, derivativeDir);

    Eigen::VectorXd _mean = meanxx +
                            (solved * (Eigen::Map<const Eigen::VectorXd>(valuesCond, numPointsCond) - mean(pointsCond, derivativeTypesCond, derivativeDirsCond, numPointsCond, derivativeDirCond)));
    Eigen::MatrixXd _cov = kxx -
                           (solved * kCx);
    MultiVariableNormalDistribution mvn(_mean, _cov);
    auto v = mvn.sample(sampler);
    std::vector<double> values;
    for (int i = 0; i < numPoints; ++i) {
        values.push_back(v(i));
    }
    return GPRealization(this, points, derivativeTypes, derivativeDirs, values.data(), numPoints, derivativeDir);
}

Eigen::VectorXd GaussianProcess::meanPrior(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const {
    Eigen::VectorXd _mean(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        Vec3d ddir_i = derivativeDirs ? derivativeDirs[i] : derivativeDir;
        _mean(i) = (*meanFunction)(derivativeTypes[i], points[i], ddir_i);
    }
    return _mean;
}

Eigen::MatrixXd GaussianProcess::covPrior(const Point3d *pointsX, const DerivativeType *derivativeTypesX, const Vec3d *derivativeDirsX, size_t numPointsX, const Vec3d &derivativeDirX,
                                          const Point3d *pointsY, const DerivativeType *derivativeTypesY, const Vec3d *derivativeDirsY, size_t numPointsY, const Vec3d &derivativeDirY) const {
    Eigen::MatrixXd _cov(numPointsX, numPointsY);
    for (int i = 0; i < numPointsX; ++i) {
        Vec3d ddir_i = derivativeDirsX ? derivativeDirsX[i] : derivativeDirX;
        for (int j = 0; j < numPointsY; ++j) {
            Vec3d ddir_j = derivativeDirsY ? derivativeDirsY[j] : derivativeDirY;
            _cov(i, j) = (*covFunction)(derivativeTypesX[i], pointsX[i], derivativeTypesY[j], pointsY[j], ddir_i, ddir_j);
        }
    }
    return _cov;
}

Eigen::MatrixXd GaussianProcess::covPriorSym(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const {
    Eigen::MatrixXd _cov(numPoints, numPoints);
    for (int i = 0; i < numPoints; ++i) {
        Vec3d ddir_i = derivativeDirs ? derivativeDirs[i] : derivativeDir;
        for (int j = 0; j <= i; ++j) {
            Vec3d ddir_j = derivativeDirs ? derivativeDirs[j] : derivativeDir;
            double cov_i_j = (*covFunction)(derivativeTypes[i], points[i], derivativeTypes[j], points[j], ddir_i, ddir_j);
            // so our kenrel should satisify cov(x,y) = cov(y,x)
            _cov(i, j) = _cov(j, i) = cov_i_j;
        }
    }
    return _cov;
}

Eigen::VectorXd GaussianProcess::mean(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const {
    if (globalCondition.size() == 0) {
        return meanPrior(points, derivativeTypes, derivativeDirs, numPoints, derivativeDir);
    }
    auto mean_prior = meanPrior(points, derivativeTypes, derivativeDirs, numPoints, derivativeDir);

    auto kCx = covPrior(EXPAND_GPREALIZATION(globalCondition), points, derivativeTypes, derivativeDirs, numPoints, derivativeDir);
    auto solved = std::visit([&kCx](auto &solver) -> Eigen::MatrixXd { return solver.solve(kCx).transpose(); }, globalCondtionSolver);
    return mean_prior +
           (solved * (Eigen::Map<const Eigen::VectorXd>(globalCondition.values.data(), globalCondition.size()) - meanPrior(EXPAND_GPREALIZATION(globalCondition))));
}

Eigen::MatrixXd GaussianProcess::cov(const Point3d *pointsX, const DerivativeType *derivativeTypesX, const Vec3d *derivativeDirsX, size_t numPointsX, const Vec3d &derivativeDirX,
                                     const Point3d *pointsY, const DerivativeType *derivativeTypesY, const Vec3d *derivativeDirsY, size_t numPointsY, const Vec3d &derivativeDirY) const {
    if (globalCondition.size() == 0) {
        return covPrior(pointsX, derivativeTypesX, derivativeDirsX, numPointsX, derivativeDirX,
                        pointsY, derivativeTypesY, derivativeDirsY, numPointsY, derivativeDirY);
    }
    auto cov_prior = covPrior(pointsX, derivativeTypesX, derivativeDirsX, numPointsX, derivativeDirX,
                              pointsY, derivativeTypesY, derivativeDirsY, numPointsY, derivativeDirY);

    auto kCx = covPrior(EXPAND_GPREALIZATION(globalCondition), pointsX, derivativeTypesX, derivativeDirsX, numPointsX, derivativeDirX);
    auto kCy = covPrior(EXPAND_GPREALIZATION(globalCondition), pointsY, derivativeTypesY, derivativeDirsY, numPointsY, derivativeDirY);
    auto solved = std::visit([&kCx](auto &solver) -> Eigen::MatrixXd { return solver.solve(kCx).transpose(); }, globalCondtionSolver);
    return cov_prior - solved * kCy;
}

Eigen::MatrixXd GaussianProcess::covSym(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const {
    if (globalCondition.size() == 0) {
        return covPriorSym(points, derivativeTypes, derivativeDirs, numPoints, derivativeDir);
    }
    auto cov_prior = covPriorSym(points, derivativeTypes, derivativeDirs, numPoints, derivativeDir);
    auto kCx = covPrior(EXPAND_GPREALIZATION(globalCondition), points, derivativeTypes, derivativeDirs, numPoints, derivativeDir);
    auto solved = std::visit([&kCx](auto &solver) -> Eigen::MatrixXd { return solver.solve(kCx).transpose(); }, globalCondtionSolver);
    return cov_prior - solved * kCx;
}

std::tuple<Eigen::VectorXd, Eigen::MatrixXd> GaussianProcess::meanAndCov(const Point3d *points, const DerivativeType *derivativeTypes, const Vec3d *derivativeDirs, size_t numPoints, const Vec3d &derivativeDir) const {
    Eigen::VectorXd _mean(numPoints);
    Eigen::MatrixXd _cov(numPoints, numPoints);
    for (int i = 0; i < numPoints; ++i) {
        Vec3d ddir_i = derivativeDirs ? derivativeDirs[i] : derivativeDir;
        _mean(i) = (*meanFunction)(derivativeTypes[i], points[i], ddir_i);
        for (int j = 0; j <= i; ++j) {
            Vec3d ddir_j = derivativeDirs ? derivativeDirs[j] : derivativeDir;
            double cov_i_j = (*covFunction)(derivativeTypes[i], points[i], derivativeTypes[j], points[j], ddir_i, ddir_j);
            // so our kenrel should satisify cov(x,y) = cov(y,x)
            _cov(i, j) = _cov(j, i) = cov_i_j;
        }
    }
    if (globalCondition.size() == 0) {
        return {_mean, _cov};
    }
    auto kCx = covPrior(EXPAND_GPREALIZATION(globalCondition), points, derivativeTypes, derivativeDirs, numPoints, derivativeDir);
    auto solved = std::visit([&kCx](auto &solver) -> Eigen::MatrixXd { return solver.solve(kCx).transpose(); }, globalCondtionSolver);
    _mean += solved * (Eigen::Map<const Eigen::VectorXd>(globalCondition.values.data(), globalCondition.size()) - meanPrior(EXPAND_GPREALIZATION(globalCondition)));
    _cov -= solved * kCx;
    return {_mean, _cov};
}

double GaussianProcess::goodStepSize(Point3d p, Vec3d rd, double desiredCov) const {
    return 0.;
}
