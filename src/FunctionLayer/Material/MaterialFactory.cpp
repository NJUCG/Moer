#include "MaterialFactory.h"


namespace  MaterialFactory{
    std::shared_ptr < Material > LoadMaterialFromJson(const nlohmann::json json) {
        std::string material_type = json.at("type");
        if ( material_type == "lambert" ) return std::make_shared < MatteMaterial >(json);
    }

    std::unordered_map < std::string, std::shared_ptr < Material>> LoadMaterialMapFromJson(const nlohmann::json json) {

        std::unordered_map < std::string, std::shared_ptr < Material>> materialMap;
        materialMap["default"] = std::make_shared < MatteMaterial >();
        if ( json.is_array() ) {
            for ( const auto & singleMaterial: json )
                materialMap[singleMaterial["name"]] = LoadMaterialFromJson(singleMaterial);
        }

        return materialMap;
    }
}