#include "EntityFactory.h"
#include "Quad.h"
#include "Sphere.h"
#include "CoreLayer/Scene/Scene.h"
#include "FunctionLayer/Light/DiffuseAreaLight.h"
namespace EntityFactory{
    std::shared_ptr < Entity > LoadEntityFromJson(const nlohmann::json &  json) {
        const std::string type = json["type"];
        std::shared_ptr < Entity > entity ;
        if(type=="quad") entity =  std::make_shared<Quad>(json);
        if(type=="sphere") entity = std::make_shared<Sphere>(json);
        {
            Vec3d emission;
            if(containsAndGet(json,"emission",emission)){
                entity->setLight(std::make_shared<DiffuseAreaLight>(entity,
                       RGB3(emission.x,emission.y,emission.z).toSpectrum()));
            }
        }
        return entity;
    }

    std::vector<std::shared_ptr < Entity >> LoadEntityListFromJson(const nlohmann::json & json,const Scene & scene){
        std::vector<std::shared_ptr < Entity >> entities;
     //   entities.push_back(std::make_shared <Sphere>(Point3d(0,0,0),1.0,nullptr));
        if(json.is_array()){
            for(auto & entityJson : json){
                entities.push_back(LoadEntityFromJson(entityJson));
            }
        }
        else{
            // error
        }
        return entities;
    }

}