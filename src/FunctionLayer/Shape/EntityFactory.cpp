#include "EntityFactory.h"
#include "Quad.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Cube.h"
#include "Curve.h"
#include "FunctionLayer/Scene/Scene.h"
#include "ResourceLayer/ResourceManager.h"
#include "ResourceLayer/File/FileUtils.h"
#include "FunctionLayer/Light/DiffuseAreaLight.h"
#include "FunctionLayer/Light/InfiniteSphereLight.h"
#include "FunctionLayer/Light/InfiniteSphereCapLight.h"

static std::unordered_map<std::string, std::string> meshNameAndMaterialNameMap(const Json &json) {
    std::unordered_map<std::string, std::string> result;
    // todo
    //     if(json.contains(""))
}

namespace EntityFactory {
bool handleEntity(std::string type, const Json &json, Scene &scene,
                  std::vector<std::shared_ptr<Entity>> &entities, int &entityCount) {

    std::shared_ptr<Entity> entity = nullptr;
    if (type == "quad") entity = std::make_shared<Quad>(json);
    if (type == "sphere") entity = std::make_shared<Sphere>(json);
    if (type == "cube") entity = std::make_shared<Cube>(json);
    if (type == "curves") entity = std::make_shared<Curve>(json);
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
    if (entity) {
        auto material = scene.fetchMaterial(
            getOptional(json, "material", std::string("default")));
        entityCount = 1;
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
    return entityCount > 0;
}

bool handleLight(std::string type, const Json &json, Scene &scene) {
    if (type == "infinite_sphere") {
        scene.addLight(std::make_shared<InfiniteSphereLight>(json));
        return true;
    }

    else if (type == "infinite_sphere_cap") {
        scene.addLight(std::make_shared<InfiniteSphereCapLight>(json));
        return true;
    }
    return true;
}

void LoadEntityFromJson(const Json &json, Scene &scene,
                        std::vector<std::shared_ptr<Entity>> &entities) {

    int entityCount = 0;
    std::string type = json["type"];
    if (handleEntity(type, json, scene, entities, entityCount))
        ;
    else
        handleLight(type, json, scene);
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