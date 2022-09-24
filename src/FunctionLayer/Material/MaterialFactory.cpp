#include "MaterialFactory.h"
#include "DielectricMaterial.h"
#include "NullMaterial.h"

namespace  MaterialFactory{
    std::shared_ptr < Material > LoadMaterialFromJson(const Json json) {
        std::string material_type = json.at("type");
        if (material_type == "lambert") return std::make_shared <MatteMaterial>(json);
        else if (material_type == "dielectric") return std::make_shared<DielectricMaterial>(json);
        else if (material_type == "null") return std::make_shared<NullMaterial>(json);

        return nullptr;
    }

    std::unordered_map < std::string, std::shared_ptr < Material>> LoadMaterialMapFromJson(const Json json) {

        std::unordered_map < std::string, std::shared_ptr < Material>> materialMap;
        materialMap["default"] = std::make_shared < MatteMaterial >();
        if ( json.is_array() ) {
            for ( const auto & singleMaterial: json )
                materialMap[singleMaterial["name"]] = LoadMaterialFromJson(singleMaterial);
        }

        return materialMap;
    }
}