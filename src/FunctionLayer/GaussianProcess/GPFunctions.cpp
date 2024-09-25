#include "GPFunctions.h"

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
