#pragma once
#include"GaussianProcess.h"
#include"GPFunctions.h"
namespace GaussianProcessFactory {
	std::shared_ptr<GaussianProcess> LoadGaussianProcessFromJson(const Json& json);
}// namespace MediumFactory
namespace MeanFunctionFactory {
	std::shared_ptr<MeanFunction> LoadMeanFunctionFromJson(const Json& json);
}
namespace CovarianceFunctionFactory {
	std::shared_ptr<CovarianceFunction> LoadCovarianceFunctionFromJson(const Json &json);
}
