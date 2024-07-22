#include "PorousLayerMicrograinMaterial.h"
#include "FunctionLayer/Texture/Texture.h"
#include "FunctionLayer/Texture/TextureFactory.h"

#include "BxDF/PorousLayerBxDF.h"
#include "BxDF/PorousLayerMicrograinBxDF.h"

#include "MaterialFactory.h"

std::shared_ptr<BxDF> PourousLayerMicrograinMaterial::getBxDF(const Intersection &intersect) const {
    Spectrum _R0 = R0->eval(intersect);
    double _tau0 = tau0->eval(intersect);
    double _beta = beta->eval(intersect);
    std::shared_ptr<MicrograinBxDF> micrograinBRDF;
    if (micrograinType == MicrograinType::CONDUCTOR) {
        micrograinBRDF = std::make_shared<ConductorMicrograinBxDF>(_R0, k, _tau0, _beta);

    } else {
        Spectrum _kd = kd->eval(intersect);
        micrograinBRDF = std::make_shared<PlasticMicrograinBxDF>(_R0, _kd, _tau0, _beta);
    }

    return std::make_shared<PourousLayerBxDF>(micrograinBRDF, bulkMaterial->getBxDF(intersect));
}

std::shared_ptr<BSSRDF> PourousLayerMicrograinMaterial::getBSSRDF(const Intersection &intersect) const {
    return std::shared_ptr<BSSRDF>();
}

PourousLayerMicrograinMaterial::PourousLayerMicrograinMaterial(const Json &json) {
    micrograinType = MicrograinType::CONDUCTOR;
    if (json.contains("micrograinType") && json["micrograinType"] == "plastic") {
        micrograinType = MicrograinType::PLASTIC;
    }
    tau0 = TextureFactory::LoadTexture<double>(json["tau0"]);
    beta = TextureFactory::LoadTexture<double>(json["beta"]);
    R0 = TextureFactory::LoadTexture<Spectrum>(json["R0"]);
    if (json.contains("k")) {
        k = json["k"];
    }
    if (json.contains("kd")) {
        kd = TextureFactory::LoadTexture<Spectrum>(json["kd"]);
    }

    bulkMaterial = MaterialFactory::LoadMaterialFromJson(json["bulkMaterial"]);
}
