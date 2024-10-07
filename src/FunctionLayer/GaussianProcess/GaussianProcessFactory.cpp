#include "GaussianProcessFactory.h"

std::shared_ptr<GaussianProcess> GaussianProcessFactory::LoadGaussianProcessFromJson(const Json &json) {
    std::string gpType = json.at("type");
    if (gpType == "std") {
        auto meanFunction = MeanFunctionFactory::LoadMeanFunctionFromJson(json["mean"]);
        auto covarianceFunction = CovarianceFunctionFactory::LoadCovarianceFunctionFromJson(json["covariance"]);
        GPRealization globalCondition;
        if (json.find("global_condition") != json.end() && getOptional(json["global_condition"], "enable", false)) {
            auto gcJson = json["global_condition"];
            auto gcPoints = gcJson["points"];
            auto gcDerivativeTypes = gcJson["derivative_types"];
            auto gcDerivativeDirs = gcJson["derivative_dirs"];
            auto gcValues = gcJson["values"];
            for (int i = 0; i < gcPoints.size(); ++i) {
                globalCondition.points.push_back(gcPoints[i]);
                if (gcDerivativeTypes[i] == "none") {
                    globalCondition.derivativeTypes.push_back(DerivativeType::None);

                } else if (gcDerivativeTypes[i] == "first") {
                    globalCondition.derivativeTypes.push_back(DerivativeType::First);
                }
                globalCondition.derivativeDirections.push_back(gcDerivativeDirs[i]);
                globalCondition.values.push_back(gcValues[i]);
            }
        }
        return std::make_shared<GaussianProcess>(meanFunction, covarianceFunction, globalCondition);
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
