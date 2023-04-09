// Kevin Egan

#include <stdio.h>
#include <stdlib.h>

#include "FunctionLayer/Material/PlasticMaterial.h"
#include "FunctionLayer/Material/MaterialFactory.h"
#include "FunctionLayer/Intersection.h"
#include "FunctionLayer/Sampler/Independent.h"
#include <fstream>
//using namespace pbrt;
//
//static MemoryArena arena;
//static RNG rng;
//
//// extract the red channel from a Spectrum class
//double spectrumRedValue(const Spectrum& s) { return s[0]; }
//
//typedef void (*CreateBSDFFunc)(BSDF* bsdf);

//void createLambertian(BSDF* bsdf);
//void createOrenNayar0(BSDF* bsdf);
//void createOrenNayar20(BSDF* bsdf);
//void createMicrofacet(BSDF* bsdf, bool beckmann, bool samplevisible,
//                      double roughx, double roughy);
//void createMicrofacet30and0(BSDF* bsdf);
//void createFresnelBlend(BSDF* bsdf, bool beckmann, bool samplevisible,
//                        double roughx, double roughy);
//
//typedef void (*GenSampleFunc)(BSDF* bsdf, const Vector3f& wo, Vector3f* wi,
//                              double* pdf, Spectrum* f);
//
//void Gen_Sample_f(BSDF* bsdf, const Vector3f& wo, Vector3f* wi, double* pdf,
//                  Spectrum* f);
//void Gen_CosHemisphere(BSDF* bsdf, const Vector3f& wo, Vector3f* wi, double* pdf,
//                       Spectrum* f);
//void Gen_UniformHemisphere(BSDF* bsdf, const Vector3f& wo, Vector3f* wi,
//                           double* pdf, Spectrum* f);

int main(int argc, char* argv[]) {
    assert(argc == 2);
    auto  filePath = std::string(argv[1]);
    const int estimates = 10000000;

    // radiance of uniform environment map
    const double environmentRadiance = 1.0;

    fprintf(stderr,
            "outgoing radiance from a surface viewed\n"
            "straight on with uniform lighting\n\n"
            "    uniform incoming radiance = %.3f\n"
            "    monte carlo samples = %d\n\n\n",
            environmentRadiance, estimates);


    const char* BSDFFuncDescripArray[] = {
        "Lambertian",
        "Oren Nayar (sigma 0)",
        "Oren Nayar (sigma 20)",
        "Beckmann (roughness 0.5, sample visible mf area)",
        "Trowbridge-Reitz (roughness 0.5, sample visible mf area)",
        "Beckmann (roughness 0.2/0.1, sample visible mf area)",
        "Trowbridge-Reitz (roughness 0.2/0.1, sample visible mf area)",
        "Beckmann (roughness 0.15/0.25, sample visible mf area)",
        "Trowbridge-Reitz (roughness 0.15/0.25, sample visible mf area)",
        "Beckmann (roughness 0.33/0.033, sample visible mf area)",
        "Trowbridge-Reitz (roughness 0.33/0.033, sample visible mf area)",
        "Beckmann (roughness 0.5, traditional sample wh)",
        "Trowbridge-Reitz (roughness 0.5, traditional sample wh)",
        "Beckmann (roughness 0.2/0.1, traditional sample wh)",
        "Trowbridge-Reitz (roughness 0.2/0.1, traditional sample wh)",
        "Beckmann (roughness 0.15/0.25, traditional sample wh)",
        "Trowbridge-Reitz (roughness 0.15/0.25, traditional sample wh)",
        "Beckmann (roughness 0.33/0.033, traditional sample wh)",
        "Trowbridge-Reitz (roughness 0.33/0.033, traditional sample wh)",
        "Fresnel Blend Beckmann (roughness 0.15/0.25, sample visible mf area)",
        "Fresnel Blend Trowbridge-Reitz (roughness 0.15/0.25, sample visible mf area)",
        "Fresnel Blend Beckmann (roughness 0.15/0.25, traditional sample wh)",
        "Fresnel Blend Trowbridge-Reitz (roughness 0.15/0.25, traditional sample wh)",
    };

//    GenSampleFunc SampleFuncArray[] = {
//        Gen_Sample_f,
//        // CO        Gen_CosHemisphere,
//        // CO        Gen_UniformHemisphere,
//    };

    const char* SampleFuncDescripArray[] = {
        "BSDF Importance Sampling",
        // CO        "Cos Hemisphere",
        // CO        "Uniform Hemisphere",
    };

//    int numModels = sizeof(BSDFFuncArray) / sizeof(BSDFFuncArray[0]);
//    int numModelsDescrip =
//        sizeof(BSDFFuncDescripArray) / sizeof(BSDFFuncDescripArray[0]);
//    int numGenerators = sizeof(SampleFuncArray) / sizeof(SampleFuncArray[0]);
//    int numGeneratorsDescrip =
//        sizeof(SampleFuncDescripArray) / sizeof(SampleFuncDescripArray[0]);

//    if (numModels != numModelsDescrip) {
//        fprintf(stderr,
//                "BSDFFuncArray and BSDFFuncDescripArray out of sync!\n");
//        exit(1);
//    }
//
//    if (numGenerators != numGeneratorsDescrip) {
//        fprintf(stderr,
//                "SampleFuncArray and SampleFuncDescripArray out of sync!\n");
//        exit(1);
//    }

    Intersection its;
    std::vector<std::shared_ptr<Material>> materials;
    Json materialJson;
    std::ifstream sceneFile(filePath);
    sceneFile>>materialJson;
    for(const auto & subJson : materialJson)
        materials.push_back(MaterialFactory::LoadMaterialFromJson(subJson));
    for(const auto & material : materials){
        auto bxdf = material->getBxDF(its);
        auto wo = Vec3d(0,0,1);
        for(int gen =  0 ;gen<1;gen++){
            double  redSum  =  0;
            const int numHistoBins = 10;
            double histogram[numHistoBins][numHistoBins];
            for (int i = 0; i < numHistoBins; i++) {
                for (int j = 0; j < numHistoBins; j++) {
                    histogram[i][j] = 0;
                }
            }
            int badSamples = 0;
            int outsideSamples = 0;
            IndependentSampler sampler;
            int warningTarget = 1;
            for (int sample = 0; sample < estimates; sample++) {
                Vec3d  wi;
                double pdf;
                Spectrum f;
                auto bxdfSampleResult = bxdf->sample(wo,sampler.sample2D(),false);
                wi = bxdfSampleResult.directionIn;
                pdf = bxdfSampleResult.pdf;
                // sample hemisphere around bsdf, wo is fixed
             //   double redF = spectrumRedValue(f);
                double  redF = 1;
                double x = std::clamp(wi.x, -1.0, 1.0);
                double y = std::clamp(wi.y, -1.0, 1.0);
                double wiPhi = (atan2(y, x) + M_PI) / (2.0 * M_PI);
                double wiCosTheta = wi.z;
                bool validSample = (wiCosTheta > 1e-7);
                if (wiPhi < -0.0001 || wiPhi > 1.0001 || wiCosTheta > 1.0001) {
                    // wiCosTheta can be less than 0
                    fprintf(stderr, "bad wi! %.3f %.3f %.3f, (%.3f %.3f)\n",
                            wi[0], wi[1], wi[2], wiPhi, wiCosTheta);
                } else if (validSample) {
                    int histoPhi = (int)(wiPhi * numHistoBins);
                    if (histoPhi == numHistoBins)
                        --histoPhi;
                    int histoCosTheta = (int)(wiCosTheta * numHistoBins);
                    if (histoCosTheta == numHistoBins)
                        --histoCosTheta;
                    assert(histoPhi >= 0 && histoPhi < numHistoBins);
                    assert(histoCosTheta >= 0 && histoCosTheta < numHistoBins);
                    histogram[histoCosTheta][histoPhi] += 1.0 / pdf;
                }

                if (!validSample) {
                    outsideSamples++;
                } else if (pdf == 0.f || std::isnan(pdf) || redF < 0 ||
                           std::isnan(redF)) {
                    if (badSamples == warningTarget) {
                        fprintf(stderr,
                                "warning %d, bad sample %d! "
                                "pdf: %.3f, redF: %.3f\n",
                                warningTarget, sample, pdf, redF);
                        warningTarget *= 10;
                    }
                    badSamples++;
                } else {
                    // outgoing radiance estimate =
                    //   bsdf * incomingRadiance * cos(wi) / pdf
                    redSum += redF * environmentRadiance * wi.z / pdf;
                }
            }
            int goodSamples = estimates - badSamples;

            // print results
            fprintf(stderr,
                    "*** BRDF: '%s', Samples: '%s'\n\n"
                    "wi histogram showing the relative weight in each bin\n"
                    "  all entries should be close to 2pi = %.5f:\n"
                    "  (%d bad samples, %d outside samples)\n\n"
                    "                          phi bins\n",
                    "plastic", "uniform",
                    M_PI * 2.0, badSamples, outsideSamples);
            double totalSum = 0.0;
            for (int i = 0; i < numHistoBins; i++) {
                fprintf(stderr, "  cos(theta) bin %02d:", i);
                for (int j = 0; j < numHistoBins; j++) {
                    fprintf(stderr, " %5.2f", histogram[i][j] * numHistoBins *
                                                  numHistoBins / goodSamples);
                    totalSum += histogram[i][j];
                }
                fprintf(stderr, "\n");
            }
            fprintf(stderr,
                    "\n  final average :  %.5f (error %.5f)\n\n"
                    "  radiance = %.5f\n\n",
                    totalSum / goodSamples, totalSum / goodSamples - M_PI * 2.0,
                    redSum / goodSamples);
        }
        }

    return 0;
}
