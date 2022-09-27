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

Scene::Scene() : lights(std::make_shared<std::vector<std::shared_ptr<Light>>>()), entities(std::make_shared<std::vector<std::shared_ptr<Entity>>>())
{
}

Scene::Scene(const Json & json) {
    materials = MaterialFactory::LoadMaterialMapFromJson(json.at("materials"));
    entities  = std::make_shared<std::vector<std::shared_ptr<Entity>>>
                        (EntityFactory::LoadEntityListFromJson(json.at("entities"),*this));
    lights = std::make_shared<std::vector<std::shared_ptr<Light>>>();
    for(std::shared_ptr<Entity> entity : *entities){
        if(entity->getLight())
            lights->push_back(entity->getLight());
    }

}

void Scene::build() {
	//accel = std::make_shared<Bvh>(*entities);
    accel = std::make_shared<EmbreeAccel>(*entities);
}
std::optional<Intersection> Scene::intersect(const Ray &r) const
{
    return accel->Intersect(r);
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



