#include "MaterialFactory.h"
#include "DielectricMaterial.h"
#include "NullMaterial.h"
#include "CoreLayer/Scene/Scene.h"

namespace  MaterialFactory{
    std::shared_ptr <Material> LoadMaterialFromJson(const Json json) {
        std::string material_type = json.at("type");
        if (material_type == "lambert") return std::make_shared <MatteMaterial>(json);
        else if (material_type == "dielectric") return std::make_shared<DielectricMaterial>(json);
        else if (material_type == "null") return std::make_shared<NullMaterial>(json);
        return nullptr;
    }

    std::unordered_map<std::string, std::shared_ptr<Material>> LoadMaterialMapFromJson(const Json & json,const Scene & scene) {

        std::unordered_map<std::string, std::shared_ptr<Material>> materialMap;
        materialMap["default"] = std::make_shared<MatteMaterial>();
        if ( json.is_array() ) {
            for ( const auto & singleMaterialJson: json )
            {
                auto singleMaterial =  LoadMaterialFromJson(singleMaterialJson);
                if(singleMaterialJson.contains("in_medium")){
                    singleMaterial->setInsideMedium(scene.fetchMedium(singleMaterialJson["in_medium"]));
                }
                if(singleMaterialJson.contains("out_medium")){
                    singleMaterial->setOutMedium(scene.fetchMedium(singleMaterialJson["out_medium"]));
                }
                materialMap[singleMaterialJson["name"]] = singleMaterial;
            }
        }

        return materialMap;
    }
}