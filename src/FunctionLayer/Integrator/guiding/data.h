#pragma once

#include <vector>
#include "CoreLayer/Geometry/Vector.h"

namespace PathGuiding {

struct SampleData {
    Vec3f position;
    Vec3f direction;
    float radiance{};
    float pdf{};
    float distance{};

    inline explicit SampleData(const Vec3d & position, const Vec3d & direction,
        double radiance, double pdf, double distance)
    {
        this->position = position.convert<float>();
        this->direction = direction.convert<float>();
        this->radiance = (float) radiance;
        this->pdf = (float) pdf;
        this->distance = (float) distance;
    }

    [[nodiscard]]
    inline bool isValid() const {
        if (position.isNaN() || direction.isNaN() || direction.isZero()) {
            return false;
        }
        if (std::isnan(radiance) || radiance <= 0) {
            return false;
        }
        if (std::isnan(pdf) || pdf <= 0) {
            return false;
        }
        if (std::isnan(distance) || distance <= 0) {
            return false;
        }
        return true;
    }
};

}
