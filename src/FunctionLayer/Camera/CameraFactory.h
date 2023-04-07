#pragma once
#include "CoreLayer/Adapter/JsonUtil.h"

class Camera;

namespace CameraFactory {
std::shared_ptr<Camera> LoadCameraFromJson(const Json &json);
}