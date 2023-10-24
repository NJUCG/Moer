#include "CameraFactory.h"
#include "Camera.h"
#include "Pinhole.h"
#include "Thinlens.h"
#include "DifferentialPinhole.h"

namespace CameraFactory {

template<class T>
std::shared_ptr<T> cameraCreatorHelper(const Json &json) {
    return std::make_shared<T>(json);
}

static std::unordered_map<std::string, std::function<std::shared_ptr<Camera>(const Json &)>> cameraMap{
    {"pinhole", cameraCreatorHelper<PinholeCamera>},
    {"thinlens", cameraCreatorHelper<ThinlensCamera>},
    {"differential_pinhole", cameraCreatorHelper<DifferentialPinholeCamera>}};

std::shared_ptr<Camera> LoadCameraFromJson(const Json &json) {
    return cameraMap[json.at("type")](json);
}
}// namespace CameraFactory