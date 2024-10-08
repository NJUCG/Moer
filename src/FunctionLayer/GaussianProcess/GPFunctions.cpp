#include "GPFunctions.h"

double CovarianceFunction::dcov_dx(const Point3d &pointX, const Point3d &pointY, const Vec3d &ddirX) const {
    autodiff::Vector3real2nd px{pointX.x, pointX.y, pointX.z};
    autodiff::Vector3real2nd py{pointY.x, pointY.y, pointY.z};
    Eigen::Array3d vx{ddirX.x, ddirX.y, ddirX.z};
    Eigen::Array3d vy(0.);
    auto dfdv = autodiff::derivatives([this](const autodiff::Vector3real2nd &x, const autodiff::Vector3real2nd &y) { return cov(x, y); }, autodiff::along(vx, vy), autodiff::at(px, py));
    return dfdv[1];
}
double CovarianceFunction::dcov_dy(const Point3d &pointX, const Point3d &pointY, const Vec3d &ddirY) const {
    autodiff::Vector3real2nd px{pointX.x, pointX.y, pointX.z};
    autodiff::Vector3real2nd py{pointY.x, pointY.y, pointY.z};
    Eigen::Array3d vx(0.);
    Eigen::Array3d vy{ddirY.x, ddirY.y, ddirY.z};
    auto dfdv = autodiff::derivatives([this](const autodiff::Vector3real2nd &x, const autodiff::Vector3real2nd &y) { return cov(x, y); }, autodiff::along(vx, vy), autodiff::at(px, py));
    return dfdv[1];
}
double CovarianceFunction::dcov2_dxdy(const Point3d &pointX, const Point3d &pointY, const Vec3d &ddirX, const Vec3d &ddirY) const {
    autodiff::Vector3dual2nd px{pointX.x, pointX.y, pointX.z};
    autodiff::Vector3dual2nd py{pointY.x, pointY.y, pointY.z};
    Eigen::Matrix3d hess = autodiff::hessian([&](const autodiff::Vector3dual2nd &px, const autodiff::Vector3dual2nd &py) { return cov(px, py); }, wrt(px, py), at(px, py)).block(3, 0, 3, 3);
    Eigen::Array3d vx{ddirX.x, ddirX.y, ddirX.z};
    Eigen::Array3d vy{ddirY.x, ddirY.y, ddirY.z};
    double res = vy.transpose().matrix() * hess * vx.matrix();
    return res;
}

ProceduralMean::ProceduralMean(const Json &json) {
    func = SdfFunctions::funcStringToEnum(json["func"]);

    Json transform = getOptional(json, "transform", Json());

    Point3d transformPos = getOptional(transform, "position", Point3d(0.0));
    transformMatrix.setTranslate(transformPos.x, transformPos.y, transformPos.z);
    Vec3d transformScale = getOptional(transform, "scale", Vec3d(1, 1, 1));
    transformMatrix.setScale(transformScale.x, transformScale.y, transformScale.z);
    Vec3d transformRot = getOptional(transform, "rotation", Vec3d(0, 0, 0));
    transformMatrix.setRotateEuler(Angle(transformRot.x, Angle::EAngleType::ANGLE_DEG),
                                   Angle(transformRot.y, Angle::EAngleType::ANGLE_DEG),
                                   Angle(transformRot.z, Angle::EAngleType::ANGLE_DEG),
                                   EulerType::EULER_YZX);

    invTransformMatrix = transformMatrix.getInverse();

    scale = getOptional(json, "scale", 1.);
    offset = getOptional(json, "offset", 0.);
}

double ProceduralMean::mean(const Point3d &point) const {
    auto invTransformedPoint = invTransformMatrix * point;
    double sd = SdfFunctions::eval(func, invTransformedPoint);
    return scale * sd + offset;
}

Vec3d ProceduralMean::dmean_dp(const Point3d &point) const {
    constexpr double eps = 0.001;

    std::array<double, 4> vals = {
        mean(point + Vec3d(eps, 0., 0.)),
        mean(point + Vec3d(0., eps, 0.)),
        mean(point + Vec3d(0., 0., eps)),
        mean(point)};

    return Vec3d(vals[0] - vals[3], vals[1] - vals[3], vals[2] - vals[3]) / eps;
}

SquaredExponentialCovariance::SquaredExponentialCovariance(const Json &json) {
    sigma = getOptional(json, "sigma", 0.01);
    lengthScale = getOptional(json, "lengthScale", 0.1);
}

autodiff::real2nd SquaredExponentialCovariance::cov(const autodiff::real2nd &dis2) const {
    return sqr(sigma) * exp(-(dis2 / (2 * sqr(lengthScale))));
}
autodiff::dual2nd SquaredExponentialCovariance::cov(const autodiff::dual2nd &dis2) const {
    return sqr(sigma) * exp(-(dis2 / (2 * sqr(lengthScale))));
}
double SquaredExponentialCovariance::cov(double dis2) const {
    return sqr(sigma) * fm::exp(-(dis2 / (2 * sqr(lengthScale))));
}
