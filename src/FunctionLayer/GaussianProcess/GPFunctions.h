#pragma once
#include "CoreLayer/Adapter/JsonUtil.h"
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Geometry/Matrix.h"

#include "FunctionLayer/SDFFunction/SdfFunctions.h"

#include "autodiff/forward/dual.hpp"
#include "autodiff/forward/real.hpp"
#include "autodiff/forward/dual/eigen.hpp"
#include "autodiff/forward/real/eigen.hpp"

enum class DerivativeType {
    None,
    First,
};
class MeanFunction {
public:
    MeanFunction() = default;
    virtual double operator()(const DerivativeType &derivativeType, const Point3d &point, const Vec3d &derivativeDir = {}) const {
        if (derivativeType == DerivativeType::None) {
            return mean(point);
        } else {
            return dot(dmean_dp(point), derivativeDir);
        }
    }

protected:
    virtual double mean(const Point3d &point) const = 0;
    virtual Vec3d dmean_dp(const Point3d &point) const = 0;
};
class CovarianceFunction {
public:
    CovarianceFunction() = default;
    virtual double operator()(const DerivativeType &derivativeTypeX, const Point3d &pointX,
                              const DerivativeType &derivativeTypeY, const Point3d &pointY,
                              const Vec3d &derivativeDirX = {}, const Vec3d &derivativeDirY = {}) const {
        if (derivativeTypeX == DerivativeType::None) {
            if (derivativeTypeY == DerivativeType::None) {
                return cov(pointX, pointY);
            } else {
                return dcov_dy(pointX, pointY, derivativeDirY);
            }
        } else {
            if (derivativeTypeY == DerivativeType::None) {
                return dcov_dx(pointX, pointY, derivativeDirX);
            } else {
                return dcov2_dxdy(pointX, pointY, derivativeDirX, derivativeDirY);
            }
        }
        return 0.;
    }

protected:
    // common interface
    virtual double cov(const Point3d &pointX, const Point3d &pointY) const = 0;
    // interfaces for autodiff
    virtual autodiff::real2nd cov(const autodiff::Vector3real1st &pointX, const autodiff::Vector3real1st &pointY) const = 0;
    virtual autodiff::dual2nd cov(const autodiff::Vector3dual2nd &pointX, const autodiff::Vector3dual2nd &pointY) const = 0;

    double dcov_dx(const Point3d &pointX, const Point3d &pointY, const Vec3d &ddirX) const {
        autodiff::Vector3real1st px{pointX.x, pointX.y, pointX.z};
        autodiff::Vector3real1st py{pointY.x, pointY.y, pointY.z};
        Eigen::Array3d vx{ddirX.x, ddirX.y, ddirX.z};
        Eigen::Array3d vy(0.);
        auto dfdv = autodiff::derivatives([this](const autodiff::Vector3real1st &x, const autodiff::Vector3real1st &y) { return cov(x, y); }, autodiff::along(vx, vy), autodiff::at(px, py));
        return dfdv[1];
    }
    double dcov_dy(const Point3d &pointX, const Point3d &pointY, const Vec3d &ddirY) const {
        autodiff::Vector3real1st px{pointX.x, pointX.y, pointX.z};
        autodiff::Vector3real1st py{pointY.x, pointY.y, pointY.z};
        Eigen::Array3d vx(0.);
        Eigen::Array3d vy{ddirY.x, ddirY.y, ddirY.z};
        auto dfdv = autodiff::derivatives([this](const autodiff::Vector3real1st &x, const autodiff::Vector3real1st &y) { return cov(x, y); }, autodiff::along(vx, vy), autodiff::at(px, py));
        return dfdv[1];
    }
    double dcov2_dxdy(const Point3d &pointX, const Point3d &pointY, const Vec3d &ddirX, const Vec3d &ddirY) const {
        autodiff::Vector3dual2nd px{pointX.x, pointX.y, pointX.z};
        autodiff::Vector3dual2nd py{pointY.x, pointY.y, pointY.z};
        Eigen::Matrix3d hess = autodiff::hessian([&](const autodiff::Vector3dual2nd &px, const autodiff::Vector3dual2nd &py) { return cov(px, py); }, wrt(px, py), at(px, py)).block(3, 0, 3, 3);
        Eigen::Array3d vx{ddirX.x, ddirX.y, ddirX.z};
        Eigen::Array3d vy{ddirY.x, ddirY.y, ddirY.z};
        double res = vy.transpose().matrix() * hess * vx.matrix();
        return res;
    }
};

class ProceduralMean : public MeanFunction {
public:
    ProceduralMean(const Json &json);

protected:
    virtual double mean(const Point3d &point) const override;
    virtual Vec3d dmean_dp(const Point3d &point) const override;

    mutable TransformMatrix3D transformMatrix;
    mutable TransformMatrix3D invTransformMatrix;

    SdfFunctions::Function func;
    double scale;
    double offset;
};
