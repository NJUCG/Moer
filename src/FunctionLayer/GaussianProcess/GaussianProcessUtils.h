#pragma once
#include "Eigen/Dense"
#include "Eigen/Sparse"
#include "CoreLayer/Geometry/Geometry.h"
#include "FunctionLayer/Sampler/Sampler.h"
#include "CoreLayer/Math/Common.h"

#define SQRT_2 1.4142135623730951
#define SQRT_2PI 2.5066282746310007

enum class DerivativeType {
    None,
    First,
};

template<typename To, typename From>
inline To vec_conv(const From &vd) {
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

inline double gaussianCDF(double mu, double s, double x) {
    return 0.5 * std::erfc(-(x - mu) / (s * SQRT_2));
}

inline double gaussianPDF(double mu, double s, double x) {
    return std::exp(-std::pow(x - mu, 2.) / (2 * s * s)) / (s * SQRT_2PI);
}

struct MultiVariableNormalDistribution {
    Eigen::VectorXd mean;

    Eigen::BDCSVD<Eigen::MatrixXd> svd;

    Eigen::MatrixXd normTransform;

    MultiVariableNormalDistribution(const Eigen::VectorXd &_mean, const Eigen::MatrixXd &_cov);

    Eigen::VectorXd sample(Sampler &sampler) const;
};
