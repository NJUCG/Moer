#include "MediumFactory.h"
#include "Homogeneous.h"
#include "Heterogeneous.h"
#include "IsotropicPhase.h"
#include "Beerslaw.h"

namespace MediumFactory {
std::shared_ptr<Medium> LoadMediumFromJson(const Json json) {
    std::string medium_type = json.at("type");
    if (medium_type == "beers_law") {
        RGB3 intensity = getOptional(json, "intensity", RGB3(1.0));
        return std::make_shared<BeerslawMedium>(intensity, std::make_shared<IsotropicPhase>());
    } else if (medium_type == "homogeneous") {
        RGB3 sigmaT = getOptional(json, "sigmaT", RGB3(0.1));
        RGB3 albedo = getOptional(json, "albedo", RGB3(0.8));
        return std::make_shared<HomogeneousMedium>(sigmaT, albedo, std::make_shared<IsotropicPhase>());
    } else if (medium_type == "heterogeneous") {
        std::string filepath = getOptional<std::string>(json, "filepath", "default");
        float sigmaScale = getOptional(json, "sigma_scale", 1.f);

        Json transform = getOptional(json, "transform", Json());
        TransformMatrix3D transformMatrix;
        Point3d pos = getOptional(transform, "position", Point3d(0.0));
        transformMatrix.setTranslate(pos.x, pos.y, pos.z);
        Vec3d scale = getOptional(transform, "scale", Vec3d(1, 1, 1));
        transformMatrix.setScale(scale.x, scale.y, scale.z);
        Vec3d rotate = getOptional(transform, "rotation", Vec3d(0, 0, 0));
        transformMatrix.setRotateEuler(Angle(rotate.x, Angle::EAngleType::ANGLE_DEG),
                                       Angle(rotate.y, Angle::EAngleType::ANGLE_DEG),
                                       Angle(rotate.z, Angle::EAngleType::ANGLE_DEG),
                                       EulerType::EULER_YZX);

        return std::make_shared<HeterogeneousMedium>(filepath, std::make_shared<IsotropicPhase>(), transformMatrix, sigmaScale);
    }
    return nullptr;
}
std::unordered_map<std::string, std::shared_ptr<Medium>> LoadMediumMapFromJson(const Json json) {
    std::unordered_map<std::string, std::shared_ptr<Medium>> mediumMap;
    if (json.is_array()) {
        for (const auto &singleMedium : json)
            mediumMap[singleMedium["name"]] = LoadMediumFromJson(singleMedium);
    }
    return mediumMap;
}
}// namespace MediumFactory
