#include "MaterialFactory.h"

#include "DisneyBSDF.h"
#include "PlasticMaterial.h"
#include "NullMaterial.h"
#include "MatteMaterial.h"
#include "MirrorMaterial.h"
#include "ConductorMaterial.h"
#include "DielectricMaterial.h"
#include "BumpMapMaterial.h"
#include "NormalMapMaterial.h"
#include "HairMaterial.h"
#include"PorousLayerMicrograinMaterial.h"

#include "FunctionLayer/Scene/Scene.h"

namespace  MaterialFactory{

std::shared_ptr <Material> LoadMaterialFromJson(const Json json) {
    std::string materialType = json.at("type");
    if (materialType == "lambert") return std::make_shared <MatteMaterial>(json);
    else if (materialType == "mirror") return std::make_shared <MirrorMaterial>();
    else if (materialType == "dielectric") return std::make_shared<DielectricMaterial>(json);
    else if(materialType == "conductor") return std::make_shared<ConductorMaterial>(json);
    else if(materialType == "plastic") return std::make_shared<PlasticMaterial>(json);
    else if(materialType == "disney") return std::make_shared<DisneyMaterial>(json);
    else if (materialType == "null") return std::make_shared<NullMaterial>(json);
    else if (materialType == "bump") return std::make_shared<BumpMaterial>(json);
    else if (materialType == "normal") return std::make_shared<NormalMapMaterial>(json);
    else if(materialType=="hair") return std::make_shared<HairMaterial>(json);
    else if (materialType == "porousLayerMicrograin") return std::make_shared<PourousLayerMicrograinMaterial>(json);
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