#include "GaussianProcess.h"
#include "CoreLayer/Geometry/Frame.h"

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

double GaussianProcess::meanZeroDownCrossingRate(const Point3d &pos, const Vec3d &ddir) {
    std::array<Point3d, 2> points = {pos, pos};
    std::array<DerivativeType, 2> derivativeTyes = {DerivativeType::None, DerivativeType::First};

    Eigen::VectorXd Mu = mean(points.data(), derivativeTyes.data(), nullptr, 2, ddir);
    Eigen::MatrixXd Sigma = covSym(points.data(), derivativeTyes.data(), nullptr, 2, ddir);

    double muX = Mu(0);
    double muXprime = Mu(1);

    double kXX = Sigma(0, 0);
    double invKXX = 1. / kXX;
    double kXprimeX = Sigma(1, 0);
    double kXXprime = Sigma(0, 1);
    double kXprimeXprime = Sigma(1, 1);

    double pdf = gaussianPDF(muX, fm::sqrt(kXX), 0);
    double muXprimeConditioned = muXprime - kXprimeX * invKXX * muXprime;
    double covXprimeConditioned = kXprimeXprime - kXprimeX * kXXprime * invKXX;

    double cdf = gaussianCDF(muXprimeConditioned, fm::sqrt(covXprimeConditioned), 0.);
    return pdf * cdf;
}

double GaussianProcess::sampleFPT(const Ray &ray, double &t, double numSampleCount, Sampler &sampler) {
    return 0.;
}

double GaussianProcess::sampleFPTCond(const Ray &ray, double &t, double numSampleCount, Sampler &sampler,
                                      const Point3d *pointsCond, const DerivativeType *derivativeTypesCond, const Vec3d *derivativeDirsCond, const double *valuesCond, size_t numPointsCond, const Vec3d &derivativeDirCond) {
    if (numPointsCond == 0) {
        return sampleFPT(ray, t, numSampleCount, sampler);
    }
    // we need the conditioned covariance and mean.
    // utilize global conditon feature
    auto transientGlobalCondition = globalCondition;
    for (int i = 0; i < numPointsCond; ++i) {
        Vec3d dir = derivativeDirsCond ? derivativeDirsCond[i] : derivativeDirCond;
        transientGlobalCondition.points.push_back(pointsCond[i]);
        transientGlobalCondition.derivativeTypes.push_back(derivativeTypesCond[i]);
        transientGlobalCondition.derivativeDirections.push_back(dir);
        transientGlobalCondition.values.push_back(valuesCond[i]);
    }
    GaussianProcess transientGP(meanFunction, covFunction, transientGlobalCondition);
    return transientGP.sampleFPT(ray, t, numSampleCount, sampler);
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
        for (int j = 0; j < numPoints; ++j) {
            Vec3d ddir_j = derivativeDirs ? derivativeDirs[j] : derivativeDir;
            double cov_i_j = (*covFunction)(derivativeTypes[i], points[i], derivativeTypes[j], points[j], ddir_i, ddir_j);
            _cov(i, j) = cov_i_j;
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

double GaussianProcess::goodStepSize(Point3d p, Vec3d rd, double desiredCov, double stepSize) const {
    return 100000;
    double stepSizeMean = stepSize;
    double stepSizeCov = stepSize;

    double unit = stepSize / 100.;
    for (int i = 1; i < 100; ++i) {
        if ((*meanFunction)(DerivativeType::None, p + rd * unit) < 0) {
            stepSizeMean = i * unit;
            break;
        }
    }

    if (desiredCov > 0.) {
        double iter = 0;
        double low = 0., high = stepSize;
        while (high - low > 1e-2 && iter < 100) {
            double mid = (low + high) / 2;
            double covResult = (*covFunction)(DerivativeType::None, p, DerivativeType::None, p + mid * rd);
            if (fm::abs(covResult - desiredCov) < 1e-2) {
                stepSize = mid;
                break;
            }
            if (covResult < desiredCov) {
                high = mid;
            } else {
                low = mid;
            }
            ++iter;
        }
    }

    return std::min(stepSizeMean, stepSizeCov);
}
