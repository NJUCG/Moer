/**
 * @file Scene.cpp
 * @author Zhimin Fan
 * @brief Scene implemention.
 * @version 0.1
 * @date 2022-05-31
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "Scene.h"
#include "FunctionLayer/Acceleration/Embree.h"
#include "FunctionLayer/Material/MaterialFactory.h"
#include "FunctionLayer/Shape/EntityFactory.h"
#include "FunctionLayer/Medium/MediumFactory.h"

Scene::Scene() : lights(std::make_shared<std::vector<std::shared_ptr<Light>>>()), entities(std::make_shared<std::vector<std::shared_ptr<Entity>>>())
{
}

Scene::Scene(const Json & json) {
    mediums =  MediumFactory::LoadMediumMapFromJson(json.at("mediums"));
    materials = MaterialFactory::LoadMaterialMapFromJson(json.at("materials"),*this);
    lights = std::make_shared<std::vector<std::shared_ptr<Light>>>();
    entities  = std::make_shared<std::vector<std::shared_ptr<Entity>>>
                        (EntityFactory::LoadEntityListFromJson(json.at("entities"),*this));

}

void Scene::build() {
    for(auto entity:*entities)
        entity->apply();
	//accel = std::make_shared<Bvh>(*entities);
    accel = std::make_shared<EmbreeAccel>(*entities);
}
std::optional<Intersection> Scene::intersect(const Ray &r) const
{
    auto its = accel->Intersect(r);
    if(its.has_value())
        its->material->setFrame(its.value(),r);
    return its;
}

std::shared_ptr<std::vector<std::shared_ptr<Light>>> Scene::getLights() const
{
    return lights;
}

void Scene::addEntity(std::shared_ptr<Entity> object)
{
    entities->push_back(object);
}

void Scene::addLight(std::shared_ptr<Light> light)
{
    lights->push_back(light);
}

std::shared_ptr < Material > Scene::fetchMaterial(const std::string & name) const{
    if(name.empty()){
        return materials.at("default");
    }
    if(!materials.count(name))
        return materials.at("default");
    return materials.at(name);
}

std::shared_ptr < Medium > Scene::fetchMedium(const std::string & name) const {
    if(!mediums.count(name)) return nullptr;
    return mediums.at(name);
}



