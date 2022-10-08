#include "MaterialFactory.h"
#include "DielectricMaterial.h"
#include "NullMaterial.h"
#include "MirrorMaterial.h"
#include "CoreLayer/Scene/Scene.h"
#include "ConductorMaterial.h"

namespace  MaterialFactory{
    std::shared_ptr <Material> LoadMaterialFromJson(const Json json) {
        std::string materialType = json.at("type");
        if (materialType == "lambert") return std::make_shared <MatteMaterial>(json);
        else if (materialType == "mirror") return std::make_shared <MirrorMaterial>();
        else if (materialType == "dielectric") return std::make_shared<DielectricMaterial>(json);
        else if(materialType == "conductor") return std::make_shared<ConductorMaterial>(json);
        else if (materialType == "null") return std::make_shared<NullMaterial>(json);
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