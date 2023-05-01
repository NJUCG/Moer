#include "GridMedium.h"
#include <FunctionLayer/Medium/Heterogeneous.h>
#include <FunctionLayer/Medium/IsotropicPhase.h>
GridMedium::GridMedium(const Json &json) : Cube(json) {

    std::string gridFilePath = getOptional<std::string>(json, "file", "");

    medium = std::make_shared<HeterogeneousMedium>(gridFilePath,
                                                   std::make_shared<IsotropicPhase>());

    Point3d pMin = medium->boxMin;
    Point3d pMax = medium->boxMax;

    this->position = .5 * (pMin + pMax);

    Vec3d diag = pMax - pMin;

    float scaleX = diag[0] * .5f;
    float scaleY = diag[1] * .5f;
    float scaleZ = diag[2] * .5f;

    this->scale = Matrix4x4::scale(scaleX, scaleY, scaleZ);
}