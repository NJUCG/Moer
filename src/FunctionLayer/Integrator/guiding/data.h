#pragma once

#include <vector>
#include "CoreLayer/Geometry/Vector.h"

namespace PathGuiding {

struct SampleData {
    Vec3d position;
    Vec3d direction;
    double radiance{};
    double pdf{};
    double distance{};

    inline explicit SampleData(const Vec3d & position, const Vec3d & direction,
        double radiance, double pdf, double distance)
    {
        this->position = position;
        this->direction = direction;
        this->radiance = radiance;
        this->pdf = pdf;
        this->distance = distance;
    }

    [[nodiscard]]
    inline bool isValid() const {
        if (std::isnan(direction[0]) || std::isnan(direction[1]) || std::isnan(direction[2]) ||
            std::isnan(position[0]) || std::isnan(position[1]) || std::isnan(position[2]))
        {
            return false;
        }
        if (direction.isZero()) {
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
