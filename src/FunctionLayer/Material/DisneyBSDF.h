/**
 * @file  DisneyBSDF.h
 * @author Junping Yuan
 * @brief  Disney Principled BSDF.Highly inspired by https://cseweb.ucsd.edu/~tzli/cse272/homework1.pdf.
 * @version 0.1
 * @date 2022.10.13
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */
#include "FunctionLayer/Material/BxDF/BxDF.h"
#include "Material.h"
#include "BxDF/MicrofacetDistribution.h"
#include "FunctionLayer/Material/BxDF/DielectricBxDF.h"

struct DisneyDiffuse;
struct DisneyMetal;
struct DisneyClearCoat;
struct DisneyGlass;
struct DisneySheen;


class DisneyBSDF : public  BxDF{
public:
    double pdf(const Vec3d & out, const Vec3d & in) const override;

    DisneyBSDF(const Spectrum & baseColor, double specularTransmission, double metallic, double subsurface,
               double specular, double roughness, double specularTint, double anisotropic, double sheen,
               double sheenTint, double clearCoat, double clearCoatGloss, double eta);
    double eta(const Vec3d &out,const Vec3d & in) const override;

    [[nodiscard]]
    double getRoughness() const override { return roughness; }

protected:
    BxDFSampleResult sample(const Vec3d & out, const Point2d & sample) const override;
    Spectrum f(const Vec3d & out, const Vec3d & in) const override;
    double  specularTransmission;
    double  metallic;
    double  specular;
    double  roughness;
    double  sheen;
    double  clearCoat;
    double ior;

    std::unique_ptr<DisneyGlass> disneyGlass;
    std::unique_ptr<DisneyDiffuse> disneyDiffuse;
    std::unique_ptr<DisneyMetal> disneyMetal;
    std::unique_ptr<DisneyClearCoat> disneyClearCoat;
    std::unique_ptr<DisneySheen> disneySheen;

};

class DisneyMaterial: public Material{
public:
    std::shared_ptr < BxDF > getBxDF(const Intersection & intersect) const override;
    std::shared_ptr < BSSRDF > getBSSRDF(const Intersection & intersect) const override;
    DisneyMaterial(const Json & json);
protected:
    std::shared_ptr<Texture<Spectrum>> baseColor;
    std::shared_ptr<Texture<double>>  specularTransmission;
    std::shared_ptr<Texture<double>>  metallic;
    std::shared_ptr<Texture<double>>  subsurface;
    std::shared_ptr<Texture<double>>  specular;
    std::shared_ptr<Texture<double>>  roughness;
    std::shared_ptr<Texture<double>>  specularTint;
    std::shared_ptr<Texture<double>>  anisotropic;
    std::shared_ptr<Texture<double>>  sheen;
    std::shared_ptr<Texture<double>>  sheenTint;
    std::shared_ptr<Texture<double>>  clearCoat;
    std::shared_ptr<Texture<double>>  clearCoatGloss;

    double eta;
};