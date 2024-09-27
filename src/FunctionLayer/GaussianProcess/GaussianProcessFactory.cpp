#include "GaussianProcessFactory.h"

std::shared_ptr<GaussianProcess> GaussianProcessFactory::LoadGaussianProcessFromJson(const Json &json) {
    std::string gpType = json.at("type");
    if (gpType == "std") {
        auto meanFunction = MeanFunctionFactory::LoadMeanFunctionFromJson(json["mean"]);
        auto covarianceFunction = CovarianceFunctionFactory::LoadCovarianceFunctionFromJson(json["covariance"]);
        return std::make_shared<GaussianProcess>(meanFunction, covarianceFunction);
    } else if (gpType == "csg") {
        // TODO(Cchen77): csg type process
        return nullptr;
    }
    return nullptr;
}

std::shared_ptr<MeanFunction> MeanFunctionFactory::LoadMeanFunctionFromJson(const Json &json) {
    std::string meanType = json.at("type");
    if (meanType == "procedural") {
        return std::make_shared<ProceduralMean>(json);
    } else {
        return nullptr;
    }
}

std::shared_ptr<CovarianceFunction> CovarianceFunctionFactory::LoadCovarianceFunctionFromJson(const Json &json) {
    std::string covType = json.at("type");
    if (covType == "squared_exponential") {
        return std::make_shared<SquaredExponentialCovariance>(json);
    } else {
        return nullptr;
    }
}
