#include "EntityFactory.h"
#include "Quad.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Cube.h"
#include "FunctionLayer/Scene/Scene.h"
#include "ResourceLayer/ResourceManager.h"
#include "ResourceLayer/File/FileUtils.h"
#include "FunctionLayer/Light/DiffuseAreaLight.h"
#include "FunctionLayer/Light/InfiniteSphereLight.h"
#include "FunctionLayer/Light/InfiniteSphereCapLight.h"
#include "FunctionLayer/Material/NullMaterial.h"
#include "FunctionLayer/Medium/Homogeneous.h"
#include "FunctionLayer/Medium/IsotropicPhase.h"

static std::unordered_map<std::string, std::string> meshNameAndMaterialNameMap(const Json &json) {
    std::unordered_map<std::string, std::string> result;
    //todo
    //    if(json.contains(""))
}

namespace EntityFactory {
void LoadEntityFromJson(const Json &json, Scene &scene,
                        std::vector<std::shared_ptr<Entity>> &entities) {
    int entityCount = 1;

    const std::string type = json["type"];
    std::shared_ptr<Entity> entity;
    if (type == "quad") entity = std::make_shared<Quad>(json);
    if (type == "sphere") entity = std::make_shared<Sphere>(json);
    //        if(type == "cube") entity = std::make_shared<Cube>(json);
    if (type == "cube") {
        entity = std::make_shared<Cube>(json);
        auto material = std::make_shared<NullMaterial>();
        material->setInsideMedium(std::make_shared<HomogeneousMedium>(
            Spectrum(RGB3(82.33059119113332 * 0.1, 144.5443633749726 * 0.1, 72.37389584908527 * 0.1)), Spectrum(RGB3(0.3129313327556703, 0.8415919222753505, 0.7871433602145848)), std::make_shared<IsotropicPhase>()));
        entity->setMaterial(material);
        entities.push_back(entity);
        return;
    }
    if (type == "mesh") {
        auto meshDataPath = FileUtils::getWorkingDir() + std::string(json["file"]);
        // for convenience. won't cost much cuz meshDataList not actually contains data.
        auto meshDataList = *(MeshDataManager::getInstance()->getMeshData(meshDataPath));
        entityCount = meshDataList.size();
        for (auto meshData : meshDataList) {
            auto material = scene.fetchMaterial(getOptional(json,
                                                            "material", std::string("default")));
            entities.push_back(std::make_shared<Mesh>(meshData.second, material, json));
        }
    }

    else if (type == "infinite_sphere") {
        entityCount = 0;
        scene.addLight(std::make_shared<InfiniteSphereLight>(json));
    }

    else if (type == "infinite_sphere_cap") {
        entityCount = 0;
        scene.addLight(std::make_shared<InfiniteSphereCapLight>(json));
    }

    else {
        auto material = scene.fetchMaterial(
            getOptional(json, "material", std::string("default")));
        entity->setMaterial(material);
        entities.push_back(entity);
    }

    {
        Vec3d emission;
        if (entityCount > 0 && containsAndGet(json, "emission", emission)) {
            int entityIdxBegin = entities.size() - entityCount;
            for (int i = 0; i < entityCount; i++) {
                entity = entities[entityIdxBegin + i];
                auto diffuseAreaLight = std::make_shared<DiffuseAreaLight>(entity,
                                                                           RGB3(emission.x, emission.y, emission.z));
                entity->setLight(diffuseAreaLight);
                scene.addLight(diffuseAreaLight);
            }
        }
    }
}

std::vector<std::shared_ptr<Entity>> LoadEntityListFromJson(const Json &json, Scene &scene) {
    std::vector<std::shared_ptr<Entity>> entities;
    //   entities.push_back(std::make_shared <Sphere>(Point3d(0,0,0),1.0,nullptr));
    if (json.is_array()) {
        for (auto &entityJson : json) {
            LoadEntityFromJson(entityJson, scene, entities);
        }
    } else {
        // error
    }
    return entities;
}

}// namespace EntityFactory