#include "Material.h"
#include "FunctionLayer/Material/BSSRDF/BSSRDF.h"
class BssrdfMaterialBase : public  Material{
public:
    BssrdfMaterialBase(const Json & json);
    std::shared_ptr < BxDF > getBxDF(const Intersection & intersect) const override final;
protected:
    std::shared_ptr <Material> bsdfMaterial;
};

class BssrdfMaterial : public BssrdfMaterialBase {
public:
    std::shared_ptr < BSSRDF > getBSSRDF(const Intersection & intersect) const override;
    BssrdfMaterial(const Json & json);

private:
    BSSRDFTable table;
    std::shared_ptr < Texture < Spectrum>> sigmaA, sigmaS;
    double eta, g;
    double scale;
};
class KdSubsurfaceMaterial : public BssrdfMaterialBase {
public:
    std::shared_ptr < BSSRDF > getBSSRDF(const Intersection & intersect) const override;
    KdSubsurfaceMaterial(const Json & json);
protected:
    BSSRDFTable table;
    std::shared_ptr < Texture < Spectrum>> Kt, mfp;
    double eta, g;
};


class BSSRDFAdapterMaterial : public Material {
    const SeparableBSSRDF *  bssrdf;
public:
    BSSRDFAdapterMaterial(const SeparableBSSRDF * bssrdf);

    std::shared_ptr < BxDF > getBxDF(const Intersection & intersect) const override;
};