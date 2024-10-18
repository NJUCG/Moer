#include "GPFunctions.h"
#include "GaussianProcessFactory.h"
#include "ResourceLayer/ResourceManager.h"
#include "ResourceLayer/File/FileUtils.h"

#include <nanovdb/util/IO.h>
#include <nanovdb/util/SampleFromVoxels.h>

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

MeanFunction::MeanFunction(const Json &json) {
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

Vec3d MeanFunction::dmean_dp(const Point3d &point) const {
    constexpr double eps = 0.001;

    std::array<double, 4> vals = {
        mean(point + Vec3d(eps, 0., 0.)),
        mean(point + Vec3d(0., eps, 0.)),
        mean(point + Vec3d(0., 0., eps)),
        mean(point)};

    return Vec3d(vals[0] - vals[3], vals[1] - vals[3], vals[2] - vals[3]) / eps;
}

ProceduralMean::ProceduralMean(const Json &json) : MeanFunction(json) {
    func = SdfFunctions::funcStringToEnum(json["func"]);
}

double ProceduralMean::mean(const Point3d &point) const {
    auto invTransformedPoint = invTransformMatrix * point;
    double sd = SdfFunctions::eval(func, invTransformedPoint);
    return scale * sd + offset;
}

TabulatedMean::TabulatedMean(const Json &json) : MeanFunction(json) {
    meanGridName = getOptional(json, "grid_name", std::string("mean"));

    std::string gridFilePath = json.at("file");
    std::string fullGridFilePath = FileUtils::getWorkingDir() + gridFilePath;
    meanGrid = nanovdb::io::readGrid(fullGridFilePath, meanGridName, 1);
    if (!meanGrid || meanGrid.gridType() != nanovdb::GridType::Float) {
        std::cerr << "tabulated mean need a \"mean\" grid with float grid type\n";
        exit(1);
    }
    meanFloatGrid = meanGrid.grid<float>();
    worldBBox = &meanFloatGrid->worldBBox();
    meanGridAccessor = std::make_shared<nanovdb::DefaultReadAccessor<float>>(meanFloatGrid->getAccessor());

    gridShouldBeNormalized = getOptional(json, "grid_should_be_normalized", false);
}

double TabulatedMean::mean(const Point3d &point) const {
    auto p = invTransformMatrix * point;
    if (gridShouldBeNormalized) {
        p *= std::max(worldBBox->max()[0] - worldBBox->min()[0], std::max(worldBBox->max()[1] - worldBBox->min()[1], worldBBox->max()[2] - worldBBox->min()[2]));
        p += Vec3d{worldBBox->min()[0], worldBBox->min()[1], worldBBox->min()[2]};
    }
    p = clamp(p, worldBBox);
    Point3d index = meanFloatGrid->worldToIndex(p);
    float res = nanovdb::SampleFromVoxels<nanovdb::DefaultReadAccessor<float>, 1, false>(*meanGridAccessor)(index);
    return scale * res + offset;
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

NonstationaryCovariance::NonstationaryCovariance(const Json &json) {
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

    {
        std::string localVarianceGridName = getOptional(json, "local_variance_grid_name", std::string("local_variance"));
        std::string gridFilePath = json.at("local_variance");
        std::string fullGridFilePath = FileUtils::getWorkingDir() + gridFilePath;
        localVarianceGrid = nanovdb::io::readGrid(fullGridFilePath, localVarianceGridName, 1);
        if (!localVarianceGrid || localVarianceGrid.gridType() != nanovdb::GridType::Float) {
            std::cerr << "invalid local variance grid!\n";
            exit(1);
        }
        localVarianceFloatGrid = localVarianceGrid.grid<float>();
        localVarianceWorldBBox = &localVarianceFloatGrid->worldBBox();
        localVarianceGridAccessor = std::make_shared<nanovdb::DefaultReadAccessor<float>>(localVarianceFloatGrid->getAccessor());
    }

    {
        std::string correlationGridName = getOptional(json, "correlation_grid_name", std::string("correlation"));
        std::string gridFilePath = json.at("correlation");
        std::string fullGridFilePath = FileUtils::getWorkingDir() + gridFilePath;
        correlationGrid = nanovdb::io::readGrid(fullGridFilePath, correlationGridName, 1);
        if (!correlationGrid || correlationGrid.gridType() != nanovdb::GridType::Float) {
            std::cerr << "invalid correlation grid!\n";
            exit(1);
        }
        correlationFloatGrid = correlationGrid.grid<float>();
        correlationWorldBBox = &correlationFloatGrid->worldBBox();
        correlationGridAccessor = std::make_shared<nanovdb::DefaultReadAccessor<float>>(correlationFloatGrid->getAccessor());
    }

    gridShouldBeNormalized = getOptional(json, "grid_should_be_normalized", false);

    stationaryCovariance = CovarianceFunctionFactory::LoadCovarianceFunctionFromJson(json["stationary_covariance"]);
    if (!dynamic_cast<StationaryCovariance *>(stationaryCovariance.get())) {
        std::cerr << "we need a stationary covariance for the \"nonstationary\" covariance!";
        exit(1);
    }
}

double NonstationaryCovariance::cov(const Point3d &pointX, const Point3d &pointY) const {
    double sigmaX = sampleLocalVariance(pointX);
    double sigmaY = sampleLocalVariance(pointY);
    double correlationX = sampleCorrelation(pointX);
    double correlationY = sampleCorrelation(pointY);

    Eigen::Vector3d px = {pointX.x, pointX.y, pointX.z};
    Eigen::Vector3d py = {pointY.x, pointY.y, pointY.z};
    auto d = px - py;

    // MARK(Cchen77): for now we just remain it correlation*identity
    // we need PSD to provide anisotropy
    Eigen::Matrix3d anisoX = correlationX * Eigen::Matrix3d::Identity();
    Eigen::Matrix3d anisoY = correlationY * Eigen::Matrix3d::Identity();

    Eigen::Matrix3d anisoAVG = (anisoX + anisoY) / 2;

    double detAnisoX = anisoX.determinant();
    double detAnisoY = anisoY.determinant();
    double detAnisoAVG = anisoAVG.determinant();

    double Q = d.transpose() * anisoAVG.inverse() * d;

    double k = (*stationaryCovariance)(DerivativeType::None, {Q, 0, 0}, DerivativeType::None, {0, 0, 0});

    return sigmaX * sigmaY * fm::pow(detAnisoX, 0.25) * fm::pow(detAnisoY, 0.25) / fm::sqrt(detAnisoAVG) * k;
}

autodiff::real2nd NonstationaryCovariance::cov(const autodiff::Vector3real2nd &pointX, const autodiff::Vector3real2nd &pointY) const {
    autodiff::real2nd sigmaX = sampleLocalVariance(pointX);
    autodiff::real2nd sigmaY = sampleLocalVariance(pointY);
    autodiff::real2nd correlationX = sampleCorrelation(pointX);
    autodiff::real2nd correlationY = sampleCorrelation(pointY);

    auto d = pointX - pointY;

    // MARK(Cchen77): for now we just remain it identity
    autodiff::Matrix3real2nd anisoX = correlationX * autodiff::Matrix3real2nd::Identity();
    autodiff::Matrix3real2nd anisoY = correlationY * autodiff::Matrix3real2nd::Identity();

    autodiff::Matrix3real2nd anisoAVG = (anisoX + anisoY) / 2;

    autodiff::real2nd detAnisoX = anisoX.determinant();
    autodiff::real2nd detAnisoY = anisoY.determinant();
    autodiff::real2nd detAnisoAVG = anisoAVG.determinant();

    autodiff::real2nd Q = d.transpose() * anisoAVG.inverse() * d;

    StationaryCovariance *covFunction = static_cast<StationaryCovariance *>(stationaryCovariance.get());
    return sigmaX * sigmaY * pow(detAnisoX, 0.25) * pow(detAnisoY, 0.25) / sqrt(detAnisoAVG) * covFunction->cov(Q);
}

autodiff::dual2nd NonstationaryCovariance::cov(const autodiff::Vector3dual2nd &pointX, const autodiff::Vector3dual2nd &pointY) const {
    autodiff::dual2nd sigmaX = sampleLocalVariance(pointX);
    autodiff::dual2nd sigmaY = sampleLocalVariance(pointY);
    autodiff::dual2nd correlationX = sampleCorrelation(pointX);
    autodiff::dual2nd correlationY = sampleCorrelation(pointY);

    auto d = pointX - pointY;

    // MARK(Cchen77): for now we just remain it identity
    autodiff::Matrix3dual2nd anisoX = correlationX * autodiff::Matrix3dual2nd::Identity();
    autodiff::Matrix3dual2nd anisoY = correlationY * autodiff::Matrix3dual2nd::Identity();

    autodiff::Matrix3dual2nd anisoAVG = (anisoX + anisoY) / 2;

    autodiff::dual2nd detAnisoX = anisoX.determinant();
    autodiff::dual2nd detAnisoY = anisoY.determinant();
    autodiff::dual2nd detAnisoAVG = anisoAVG.determinant();

    autodiff::dual2nd Q = d.transpose() * anisoAVG.inverse() * d;

    StationaryCovariance *covFunction = static_cast<StationaryCovariance *>(stationaryCovariance.get());
    return sigmaX * sigmaY * pow(detAnisoX, 0.25) * pow(detAnisoY, 0.25) / sqrt(detAnisoAVG) * covFunction->cov(Q);
}

double NonstationaryCovariance::sampleLocalVariance(const Point3d &point) const {
    Point3d p = invTransformMatrix * point;
    if (gridShouldBeNormalized) {
        p *= std::max(localVarianceWorldBBox->max()[0] - localVarianceWorldBBox->min()[0],
                      std::max(localVarianceWorldBBox->max()[1] - localVarianceWorldBBox->min()[1], localVarianceWorldBBox->max()[2] - localVarianceWorldBBox->min()[2]));
        p += Vec3d{localVarianceWorldBBox->min()[0], localVarianceWorldBBox->min()[1], localVarianceWorldBBox->min()[2]};
    }
    p = clamp(p, localVarianceWorldBBox);
    Point3d index = localVarianceFloatGrid->worldToIndex(p);
    float res = nanovdb::SampleFromVoxels<nanovdb::DefaultReadAccessor<float>, 1, false>(*localVarianceGridAccessor)(index);
    return res;
}

autodiff::real2nd NonstationaryCovariance::sampleLocalVariance(const autodiff::Vector3real2nd &point) const {
    Point3d p = {(double)point.x(), (double)point.y(), (double)point.z()};
    autodiff::real2nd result;
    result[0] = sampleLocalVariance(p);

    double eps = 0.001;
    std::array<double, 4> vals = {
        sampleLocalVariance(p + Vec3d(eps, 0., 0.)),
        sampleLocalVariance(p + Vec3d(0., eps, 0.)),
        sampleLocalVariance(p + Vec3d(0., 0., eps)),
        result[0]};

    auto grad = Vec3d{vals[0] - vals[3], vals[1] - vals[3], vals[2] - vals[3]} / eps;

    result[1] = dot(grad, Vec3d{(double)point.x()[1], (double)point.y()[1], (double)point.z()[1]});
    // since we use linear interpolation,the second order derivative is always 0
    result[2] = 0;
    return result;
}

autodiff::dual2nd NonstationaryCovariance::sampleLocalVariance(const autodiff::Vector3dual2nd &point) const {
    Point3d p = {(double)point.x(), (double)point.y(), (double)point.z()};
    autodiff::dual2nd result;
    result.val = sampleLocalVariance(p);

    double eps = 0.001;
    std::array<double, 4> vals = {
        sampleLocalVariance(p + Vec3d(eps, 0., 0.)),
        sampleLocalVariance(p + Vec3d(0., eps, 0.)),
        sampleLocalVariance(p + Vec3d(0., 0., eps)),
        (double)result.val};

    auto grad = Vec3d{vals[0] - vals[3], vals[1] - vals[3], vals[2] - vals[3]} / eps;

    result.grad.val = dot(grad, Vec3d{point.x().grad.val, point.y().grad.val, point.z().grad.val});
    // since we use linear interpolation,the second order derivative is always 0
    result.grad.grad = 0;
    return result;
}

double NonstationaryCovariance::sampleCorrelation(const Point3d &point) const {
    Point3d p = invTransformMatrix * point;
    if (gridShouldBeNormalized) {
        p *= std::max(correlationWorldBBox->max()[0] - correlationWorldBBox->min()[0],
                      std::max(correlationWorldBBox->max()[1] - correlationWorldBBox->min()[1], correlationWorldBBox->max()[2] - correlationWorldBBox->min()[2]));
        p += Vec3d{correlationWorldBBox->min()[0], correlationWorldBBox->min()[1], correlationWorldBBox->min()[2]};
    }
    p = clamp(p, correlationWorldBBox);
    Point3d index = correlationFloatGrid->worldToIndex(p);
    float res = nanovdb::SampleFromVoxels<nanovdb::DefaultReadAccessor<float>, 1, false>(*correlationFloatGrid)(index);
    return res;
}

autodiff::real2nd NonstationaryCovariance::sampleCorrelation(const autodiff::Vector3real2nd &point) const {
    Point3d p = {(double)point.x(), (double)point.y(), (double)point.z()};
    autodiff::real2nd result;
    result[0] = sampleCorrelation(p);

    double eps = 0.001;
    std::array<double, 4> vals = {
        sampleCorrelation(p + Vec3d(eps, 0., 0.)),
        sampleCorrelation(p + Vec3d(0., eps, 0.)),
        sampleCorrelation(p + Vec3d(0., 0., eps)),
        result[0]};

    auto grad = Vec3d{vals[0] - vals[3], vals[1] - vals[3], vals[2] - vals[3]} / eps;

    result[1] = dot(grad, Vec3d{(double)point.x()[1], (double)point.y()[1], (double)point.z()[1]});
    // since we use linear interpolation,the second order derivative is always 0
    result[2] = 0;
    return result;
}

autodiff::dual2nd NonstationaryCovariance::sampleCorrelation(const autodiff::Vector3dual2nd &point) const {
    Point3d p = {(double)point.x(), (double)point.y(), (double)point.z()};
    autodiff::dual2nd result;
    result.val = sampleCorrelation(p);

    double eps = 0.001;
    std::array<double, 4> vals = {
        sampleCorrelation(p + Vec3d(eps, 0., 0.)),
        sampleCorrelation(p + Vec3d(0., eps, 0.)),
        sampleCorrelation(p + Vec3d(0., 0., eps)),
        (double)result.val};

    auto grad = Vec3d{vals[0] - vals[3], vals[1] - vals[3], vals[2] - vals[3]} / eps;

    result.grad.val = dot(grad, Vec3d{point.x().grad.val, point.y().grad.val, point.z().grad.val});
    // since we use linear interpolation,the second order derivative is always 0
    result.grad.grad = 0;
    return result;
}
