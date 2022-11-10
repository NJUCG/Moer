#include "CoreLayer/Adapter/JsonUtil.hpp"

class Camera;

namespace CameraFactory {
    std::shared_ptr<Camera> LoadCameraFromJson(const Json & json);
}