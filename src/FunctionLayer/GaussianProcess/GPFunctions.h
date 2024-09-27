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
    virtual autodiff::real2nd cov(const autodiff::Vector3real2nd &pointX, const autodiff::Vector3real2nd &pointY) const = 0;
    virtual autodiff::dual2nd cov(const autodiff::Vector3dual2nd &pointX, const autodiff::Vector3dual2nd &pointY) const = 0;

    double dcov_dx(const Point3d &pointX, const Point3d &pointY, const Vec3d &ddirX) const;
    double dcov_dy(const Point3d &pointX, const Point3d &pointY, const Vec3d &ddirY) const;
    double dcov2_dxdy(const Point3d &pointX, const Point3d &pointY, const Vec3d &ddirX, const Vec3d &ddirY) const;
};

class StationaryCovariance : public CovarianceFunction {
protected:
    virtual autodiff::real2nd cov(const autodiff::real2nd &dis2) const = 0;
    virtual autodiff::dual2nd cov(const autodiff::dual2nd &dis2) const = 0;
    virtual double cov(double dis) const = 0;

    virtual double cov(const Point3d &pointX, const Point3d &pointY) const {
        auto d = pointX - pointY;
        return cov(dot(d, d));
    }
    virtual autodiff::real2nd cov(const autodiff::Vector3real2nd &pointX, const autodiff::Vector3real2nd &pointY) const {
        auto d = pointX - pointY;
        return cov(d.dot(d));
    }
    virtual autodiff::dual2nd cov(const autodiff::Vector3dual2nd &pointX, const autodiff::Vector3dual2nd &pointY) const {
        auto d = pointX - pointY;
        return cov(d.dot(d));
    }
};

class SquaredExponentialCovariance : public StationaryCovariance {
public:
    SquaredExponentialCovariance(const Json &json);

protected:
    virtual autodiff::real2nd cov(const autodiff::real2nd &dis2) const override;
    virtual autodiff::dual2nd cov(const autodiff::dual2nd &dis2) const override;
    virtual double cov(double dis2) const;

protected:
    double sigma;
    double lengthScale;
};
