#include "MediumFactory.h"
#include "Homogeneous.h"
#include "IsotropicPhase.h"
#include "Beerslaw.h"

namespace  MediumFactory{
    std::shared_ptr < Medium > LoadMediumFromJson(const Json json) {
        std::string medium_type = json.at("type");
        if (medium_type == "beers_law")
        {
            RGB3 intensity = getOptional(json,"intensity",RGB3(1.0));
            return std::make_shared <BeerslawMedium>(intensity,std::make_shared <IsotropicPhase>());
            // return std::make_shared <BeerslawMedium>(intensity.toSpectrum(),std::make_shared <IsotropicPhase>());
        }
        else if (medium_type == "homogeneous"){
            RGB3 sigmaT = getOptional(json,"sigmaT",RGB3(0.1));
            RGB3 albedo = getOptional(json,"albedo",RGB3(0.8));
            return std::make_shared <HomogeneousMedium>(sigmaT,albedo,std::make_shared <IsotropicPhase>());
        }
        return nullptr;
    }
    std::unordered_map < std::string, std::shared_ptr < Medium>> LoadMediumMapFromJson(const Json json) {
        std::unordered_map<std::string, std::shared_ptr<Medium>> mediumMap;
        if ( json.is_array() ) {
            for ( const auto & singleMedium: json )
                mediumMap[singleMedium["name"]] = LoadMediumFromJson(singleMedium);
        }
        return mediumMap;
    }
}
