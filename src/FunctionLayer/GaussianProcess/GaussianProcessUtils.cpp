#include"GaussianProcessUtils.h"

// https://github.com/daseyb/gpis-light-transport/blob/main/src/core/sampling/Gaussian.cpp
MultiVariableNormalDistribution::MultiVariableNormalDistribution(const Eigen::VectorXd &_mean, const Eigen::MatrixXd &_cov) {

    Eigen::LLT<Eigen::MatrixXd> chol(_cov.triangularView<Eigen::Lower>());
    // We can only use the cholesky decomposition if
    // the covariance matrix is symmetric, pos-definite.
    // But a covariance matrix might be pos-semi-definite.
    // In that case, we'll go to an EigenSolver
    if (chol.info() == Eigen::Success) {
        // Use cholesky solver
        normTransform = chol.matrixL();
    } else {
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigs(_cov);

        if (eigs.info() != Eigen::ComputationInfo::Success) {
            std::cerr << "Matrix square root failed!\n";
        }

        normTransform = eigs.eigenvectors() * eigs.eigenvalues().cwiseMax(0).cwiseSqrt().asDiagonal();
    }

    mean = _mean;
}

// https://github.com/daseyb/gpis-light-transport/blob/main/src/core/sampling/Gaussian.cpp
Eigen::VectorXd MultiVariableNormalDistribution::sample(Sampler &sampler) const {
    // Generate a vector of standard normal variates with the same dimension as the mean
    Eigen::VectorXd z = Eigen::VectorXd(mean.size());

    // We're always getting two samples, so make use of that
    for (int i = 0; i < mean.size() / 2; i++) {
        Vec2d norm_samp = rand_normal_2(sampler);
        z(i * 2) = norm_samp.x;
        z(i * 2 + 1) = norm_samp.y;
    }
    if (mean.size() % 2)
    {
        Vec2d norm_samp = rand_normal_2(sampler);
        z(mean.size() - 1) = norm_samp.x;
    }
    Eigen::VectorXd sample = mean + normTransform * z;
    return sample;
}
