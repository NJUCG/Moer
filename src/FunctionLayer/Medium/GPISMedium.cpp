#include "GPISMedium.h"
#include "GPISPhase.h"
#include "FunctionLayer/Sampler/Independent.h"
#include"FunctionLayer/GaussianProcess/GaussianProcessFactory.h"


GPISMedium::GPISMedium(const Json &json) : Medium(std::make_shared<GPISPhase>(json["phase"])) {
    numSamplePoints = getOptional(json, "num_sample_points", 8);
    marchingStepSize = getOptional(json, "marching_step_size", 0.1);

    gaussianProcess = GaussianProcessFactory::LoadGaussianProcessFromJson(json["gaussian_process"]);
}

bool GPISMedium::sampleDistance(MediumSampleRecord *mRec, const Ray &ray, const Intersection &its, Point2d sample) const {
#if defined(ENABLE_GPISMEDIUM)
    GPRealization &gpRealization = mRec->mediumState->realization;
    Sampler &sampler = mRec->mediumState->sampler;

    Ray r = ray;
    r.timeMax = std::min(its.t, r.timeMax);
    double t = r.timeMin;
    bool intersected = false;
    do {
        intersected = intersectGP(r, gpRealization, t, sampler);
        if (t < r.timeMax) {
            Point3d point = r.origin + t * r.direction;
            Vec3d grad = gpRealization.sampleGradient(point, r.direction, sampler);
            if (intersected) {
                mRec->aniso = grad;
                mRec->marchLength = t;
                mRec->scatterPoint = point;
            }
            gpRealization.applyMemoryModel(r.direction, memoryModel);
        }
    } while (!intersected && t < r.timeMax);
    mRec->sigmaS = 1.;
    mRec->sigmaA = 0.;
    mRec->pdf = 1.;
    mRec->tr = 1.;
    mRec->needAniso = true;
    return intersected;

#else
    return false;
#endif
}

Spectrum GPISMedium::evalTransmittance(Point3d from, Point3d dest) const {
    // TODO(Cchen77): 
    // limited by vol path tracer framework,we just have a naive version,which just like apply Renewal memory model before cast the shadowray
    // need a more elegant way

    IndependentSampler transientSampler(1, 5);// 1,5 is meaningless

    Vec3d direction = (dest - from);
    if (direction.isZero()) {
        return 1.;
    }
    direction = normalize(direction);
    Ray ray{from, direction};
    MediumState mediumState{transientSampler};
    mediumState.realization.reset();
    mediumState.realization.points.push_back(dest);
    mediumState.realization.values.push_back(0);
    mediumState.realization.derivativeTypes.push_back(DerivativeType::None);
    mediumState.realization.derivativeDirections.push_back({});

    MediumSampleRecord sampleRecord{};
    sampleRecord.mediumState = &mediumState;

    Intersection its;
    its.t = (dest - from)[0] / direction[0];
    bool shadowed = sampleDistance(&sampleRecord, ray, its, {});

    return 1 - shadowed;
}

bool GPISMedium::intersectGP(const Ray &ray, GPRealization &gpRealization, double &t, Sampler &sampler) const {
    double maxDistance = ray.timeMax - t;
    double determinedStepSize = maxDistance / (numSamplePoints - 1);
    if (marchingStepSize < determinedStepSize) {
        determinedStepSize = marchingStepSize;
    }

    std::vector<Point3d> points(numSamplePoints);
    std::vector<DerivativeType> derivativeTypes;
    std::vector<double> ts;

    for (int i = 0; i < numSamplePoints; ++i) {
        Point3d p = ray.origin + ray.direction * (i * determinedStepSize + t);
        points.push_back(p);
        derivativeTypes.push_back(DerivativeType::None);
        ts.push_back(i * determinedStepSize + t);
    }

    // it's the first time we have a realization,so we can sampling without condition
    if (gpRealization.isEmpty()) {
        gpRealization = gaussianProcess->sample(points.data(), derivativeTypes.data(), nullptr, numSamplePoints, {}, sampler);
    }
    // use last realization as conditon
    else {
        gpRealization = gaussianProcess->sampleCond(
            points.data(), derivativeTypes.data(), nullptr, numSamplePoints, {}, gpRealization, sampler);
    }

    double lastV = gpRealization.values[0];
    double lastT = ray.timeMin;
    t = ts[0];
    for (int i = 1; i < numSamplePoints; ++i) {
        double curV = gpRealization.values[i];
        double curT = ts[i];
        if (curV * lastV <= 0.) {
            double offset = curV / (curV - lastV);
            gpRealization.makeIntersection(i, offset);
            t = lerp(lastT, curT, offset);
            return true;
        }
        t = ts[i];
        lastV = curV;
        lastT = curT;
    }
    return false;
}
