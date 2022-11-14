#include "CameraFactory.h"
#include "Camera.h"
#include "Pinhole.h"

namespace CameraFactory {

    template < class T >
    std::shared_ptr < T > cameraCreatorHelper(const Json & json) {
        return std::make_shared < T >(json);
    }

    static std::unordered_map < std::string, std::function < std::shared_ptr < Camera >(const Json &)>> cameraMap
            {
                    {"pinhole", cameraCreatorHelper < PinholeCamera >}
            };

    std::shared_ptr < Camera > LoadCameraFromJson(const Json & json) {
        return cameraMap[json["type"]](json);
    }
}