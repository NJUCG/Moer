#pragma once
#include "CoreLayer/Adapter/JsonUtil.h"
#include "CoreLayer/Geometry/Geometry.h"
enum class DerivativeType;
class MeanFunction {
public:
    MeanFunction() = default;
    MeanFunction(const Json &json);
    virtual double operator()(const DerivativeType &derivativeType, const Point3d &point, const Vec3d &derivativeDir = {}) const { return 0.; }
};
class CovarianceFunction {
public:
    CovarianceFunction() = default;
    CovarianceFunction(const Json &json);
    virtual double operator()(const DerivativeType &derivativeTypeX, const Point3d &pointX,
                              const DerivativeType &derivativeTypeY, const Point3d &pointY,
                              const Vec3d &derivativeDirX = {}, const Vec3d &derivativeDirY = {}) const { return 0.; }
};