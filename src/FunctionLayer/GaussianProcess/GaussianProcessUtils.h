#pragma once
#include"Eigen/Dense"
#include"Eigen/Sparse"
#include"CoreLayer/Geometry/Geometry.h"
#include"FunctionLayer/Sampler/Sampler.h"
#include"CoreLayer/Math/Common.h"

template<typename To,typename From>
inline To vec_conv(const From& vd) {
    return To{vd.x, vd.y, vd.x};
}
// Box muller transform
inline Vec2d rand_normal_2(Sampler &sampler) {
    double u1 = sampler.sample1D();
    double u2 = sampler.sample1D();

    double r = fm::sqrt(-2 * log(1. - u1));
    double x = fm::cos(2 * M_PI * u2);
    double y = fm::sin(2 * M_PI * u2);
    double z1 = r * x;
    double z2 = r * y;

    return Vec2d(z1, z2);
}

inline Eigen::MatrixXd project_to_psd(const Eigen::MatrixXd& in) {
   
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(in);
    Eigen::VectorXd eigenValues = es.eigenvalues();
    for (int i = 0; i < eigenValues.size(); ++i) {
        if (eigenValues(i) < 0) {
            eigenValues(i) = 0;
        }
    }
    Eigen::MatrixXd psdMatrix = es.eigenvectors() * eigenValues.asDiagonal() * es.eigenvectors().transpose();
    return psdMatrix;

}

struct MultiVariableNormalDistribution {
    Eigen::VectorXd mean;

    Eigen::BDCSVD<Eigen::MatrixXd> svd;

    Eigen::MatrixXd normTransform;

    MultiVariableNormalDistribution(const Eigen::VectorXd &_mean, const Eigen::MatrixXd &_cov);

    Eigen::VectorXd sample(Sampler &sampler) const;
};