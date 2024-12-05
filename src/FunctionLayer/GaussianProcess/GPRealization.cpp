#include "GPRealization.h"
#include "CoreLayer/Geometry/Frame.h"
#include "GaussianProcess.h"

GPRealization::GPRealization(const GaussianProcess *_gp,
                             const Point3d *_points,
                             const DerivativeType *_derivativeTypes,
                             const Vec3d *_derivativeDirs,
                             const double *_values,
                             size_t numPoints, const Vec3d &derivativeDir) : gp(_gp) {
    for (int i = 0; i < numPoints; ++i) {
        Vec3d ddir = _derivativeDirs ? _derivativeDirs[i] : derivativeDir;
        points.push_back(_points[i]);
        derivativeTypes.push_back(_derivativeTypes[i]);
        derivativeDirections.push_back(ddir);
        values.push_back(_values[i]);
    }
}

void GPRealization::makeIntersection(size_t p, double offset) {
    auto preV = values[p - 1];
    auto curV = values[p];

    Point3d zeroCrossing = lerp(points[p - 1], points[p], offset);
    double gradValue = (curV - preV) / ((points[p] - points[p - 1]).length());

    points.push_back(zeroCrossing);
    derivativeTypes.push_back(DerivativeType::None);
    values.push_back(lerp(preV, curV, offset));
    // place holder
    derivativeDirections.push_back({});

    points.push_back(zeroCrossing);
    derivativeTypes.push_back(DerivativeType::First);
    values.push_back(gradValue);
    derivativeDirections.push_back(normalize(points[p] - points[p - 1]));

    justIntersected = true;
}

void GPRealization::manualIntersectionAndNormal(Point3d pos, Vec3d rayDir, double grad) {
    // intersection
    points.push_back(pos);
    derivativeTypes.push_back(DerivativeType::None);
    derivativeDirections.push_back({});
    values.push_back(0.);
    // grad
    points.push_back(pos);
    derivativeTypes.push_back(DerivativeType::First);
    derivativeDirections.push_back(rayDir);
    values.push_back(grad);

    justIntersected = true;
}

Vec3d GPRealization::sampleGradient(Point3d pos, Vec3d rayDir, Sampler &sampler) {
    std::array<Point3d, 3> gradPs{pos, pos, pos};
    std::array<DerivativeType, 3> gradDerivs{DerivativeType::First, DerivativeType::First, DerivativeType::First};

    Frame frame(rayDir);

    Vec3d sampleGrad = {};

    // we just apply 'makeIntersect' method
    if (justIntersected) {
        std::array<Vec3d, 2> gradDirs{
            vec_conv<Vec3d>(frame.s),
            vec_conv<Vec3d>(frame.t)};
        auto realization = gp->sampleCond(gradPs.data(), gradDerivs.data(), gradDirs.data(), gradDirs.size(), {},
                                          points.data(), derivativeTypes.data(), derivativeDirections.data(), values.data(), points.size(), {}, sampler);
        // intersection's gradient is already known since we perform linear interpolation between points
        sampleGrad = frame.toWorld({realization.values[0], realization.values[1], values[values.size() - 1]});

    } else {
        std::array<Vec3d, 3> gradDirs{
            vec_conv<Vec3d>(frame.s),
            vec_conv<Vec3d>(frame.t),
            vec_conv<Vec3d>(frame.n)};
        auto realization = gp->sampleCond(gradPs.data(), gradDerivs.data(), gradDirs.data(), gradDirs.size(), {},
                                          points.data(), derivativeTypes.data(), derivativeDirections.data(), values.data(), points.size(), {}, sampler);
        sampleGrad = frame.toWorld({realization.values[0], realization.values[1], realization.values[2]});
    }
    return lastSampledGrad = sampleGrad;
}

void GPRealization::applyMemoryModel(Vec3d rayDir, MemoryModel memoryModel) {
    std::vector<Point3d> pointsNew;
    std::vector<Vec3d> derivativeDirectionsNew;
    std::vector<DerivativeType> derivativeTypesNew;
    std::vector<double> valuesNew;

    size_t pointSize = points.size();
    switch (memoryModel) {
        case MemoryModel::None:
            break;
        case MemoryModel::GlobalN:
            // TODO(Cchen77): GlobalN memory model
            break;
        case MemoryModel::Renewal: {
            size_t p = pointSize - 1;
            if (justIntersected) {
                p = pointSize - 2;
            }
            pointsNew.push_back(points[p]);
            derivativeDirectionsNew.push_back(derivativeDirections[p]);
            derivativeTypesNew.push_back(derivativeTypes[p]);
            valuesNew.push_back(values[p]);

            points = pointsNew;
            derivativeDirections = derivativeDirectionsNew;
            derivativeTypes = derivativeTypesNew;
            values = valuesNew;

            break;
        }
        case MemoryModel::RenewalPlus: {
            size_t p = pointSize - 1;
            if (justIntersected) {
                p = pointSize - 2;
            }
            pointsNew.push_back(points[p]);
            derivativeDirectionsNew.push_back({});
            derivativeTypesNew.push_back(DerivativeType::None);
            valuesNew.push_back(values[p]);

            pointsNew.push_back(points[p]);
            derivativeDirectionsNew.push_back(rayDir);
            derivativeTypesNew.push_back(DerivativeType::First);
            valuesNew.push_back(dot(lastSampledGrad, rayDir));

            points = pointsNew;
            derivativeDirections = derivativeDirectionsNew;
            derivativeTypes = derivativeTypesNew;
            values = valuesNew;

            break;
        }
        case MemoryModel::Clear: {
            points.clear();
            derivativeTypes.clear();
            derivativeDirections.clear();
            values.clear();

            break;
        }
    }
    justIntersected = false;
    lastSampledGrad = {};
}