/**
 * @file VolPathIntegrator.cpp
 * @author Chenxi Zhou
 * @brief 
 * @version 0.1
 * @date 2022-09-22
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#include "VolPathIntegrator.h"
#include "CoreLayer/Math/Warp.h"
#include "FastMath.h"
#include "FunctionLayer/Material/NullMaterial.h"

VolPathIntegrator::VolPathIntegrator(std::shared_ptr<Camera> _camera,
                                     std::unique_ptr<Film> _film,
                                     std::unique_ptr<TileGenerator> _tileGenerator,
                                     std::shared_ptr<Sampler> _sampler,
                                     int _spp,
                                     int _threadThreadNum)
    : AbstractPathIntegrator(_camera, std::move(_film),
                             std::move(_tileGenerator),
                             _sampler, _spp,
                             _threadThreadNum) {
}

Spectrum VolPathIntegrator::Li(const Ray &initialRay, std::shared_ptr<Scene> scene) {
    Spectrum L{.0};
    Spectrum throughput{1.0};

    Ray ray = initialRay;

    const double eps = 1e-4;
    int nBounces = 0;
    bool specularBounce = false;
    PathIntegratorLocalRecord prevLightSampleRecord;

    std::shared_ptr<Medium> medium = nullptr;
    auto itsOpt = scene->intersect(ray);

    while (true) {

        MediumSampleRecord mRec;
        if (medium && medium->sampleDistance(&mRec, ray, itsOpt.value(), sampler->sample2D())) {
            // Handle medium distance sampling
            throughput *= mRec.tr * mRec.sigmaS / mRec.pdf;
            Intersection mediumScatteringPoint = fulfillScatteringPoint(mRec.scatterPoint, ray.direction, medium);
            //* ----- Luminaire Sampling -----
            for (int i = 0; i < nDirectLightSamples; ++i) {
                PathIntegratorLocalRecord sampleLightRecord = sampleDirectLighting(scene, mediumScatteringPoint, ray);
                PathIntegratorLocalRecord evalScatterRecord = evalScatter(mediumScatteringPoint, ray, sampleLightRecord.wi);
                if (!sampleLightRecord.f.isBlack()) {
                    double misw = MISWeight(sampleLightRecord.pdf, evalScatterRecord.pdf);
                    if (sampleLightRecord.isDelta)
                        misw = 1.0;
                    L += throughput * sampleLightRecord.f * evalScatterRecord.f / sampleLightRecord.pdf * misw / nDirectLightSamples;
                }
            }

            //* ----- Phase Sampling -----

            PathIntegratorLocalRecord sampleScatterRecord = sampleScatter(mediumScatteringPoint, ray);
            if (sampleScatterRecord.f.isBlack())
                break;
            throughput *= sampleScatterRecord.f / sampleScatterRecord.pdf;
            ray = Ray{mediumScatteringPoint.position + sampleScatterRecord.wi * eps, sampleScatterRecord.wi};
            itsOpt = scene->intersect(ray);
            auto [sampleIts, tr] = intersectIgnoreSurface(scene, ray, medium);
            auto evalLightRecord = evalEmittance(scene, sampleIts, ray);
            if (!evalLightRecord.f.isBlack()) {
                double misw = MISWeight(sampleScatterRecord.pdf, evalLightRecord.pdf);
                if (sampleScatterRecord.isDelta)
                    misw = 1.0;
                L += throughput * tr * evalLightRecord.f * misw;
            }
        } else {
            if (medium) throughput *= mRec.tr / mRec.pdf;

            {
                PathIntegratorLocalRecord evalLightRecord = evalEmittance(scene, itsOpt, ray);
                if (nBounces == 0) {
                    L += throughput * evalLightRecord.f;
                    if (!itsOpt) break;
                }
            }

            auto its = itsOpt.value();
            its.medium = medium;

            if (its.material->getBxDF(its)->isNull()) {
                medium = getTargetMedium(its, ray.direction);
                ray = Ray{its.position + eps * ray.direction, ray.direction};
                itsOpt = scene->intersect(ray);
                if (!itsOpt) break;
                continue;
            }

            //* Direct Illumination
            for (int i = 0; i < nDirectLightSamples; ++i) {
                PathIntegratorLocalRecord sampleLightRecord = sampleDirectLighting(scene, its, ray);
                PathIntegratorLocalRecord evalScatterRecord = evalScatter(its, ray, sampleLightRecord.wi);

                if (!sampleLightRecord.f.isBlack()) {
                    //* Multiple importance sampling
                    double misw = MISWeight(sampleLightRecord.pdf, evalScatterRecord.pdf);
                    if (sampleLightRecord.isDelta)
                        misw = 1.0;
                    L += throughput * sampleLightRecord.f * evalScatterRecord.f / sampleLightRecord.pdf * misw / nDirectLightSamples;
                }
            }

            //* ----- BSDF Sampling -----
            PathIntegratorLocalRecord sampleScatterRecord = sampleScatter(its, ray);
            if (sampleScatterRecord.f.isBlack())
                break;
            throughput *= sampleScatterRecord.f / sampleScatterRecord.pdf;

            //* Test whether the sampling ray hit the emitter
            const double eps = 1e-4;
            ray = Ray{its.position + sampleScatterRecord.wi * eps, sampleScatterRecord.wi};
            itsOpt = scene->intersect(ray);

            auto [sampleIts, tr] = intersectIgnoreSurface(scene, ray, medium);

            auto evalLightRecord = evalEmittance(scene, sampleIts, ray);
            if (!evalLightRecord.f.isBlack()) {
                //* The sampling ray hit the emitter
                //* Multiple importance sampling
                double misw = MISWeight(sampleScatterRecord.pdf, evalLightRecord.pdf);
                if (sampleScatterRecord.isDelta)
                    misw = 1.0;
                L += throughput * tr * evalLightRecord.f * misw;
            }
        }
        nBounces++;
        if (nBounces > nPathLengthLimit || !itsOpt) break;
        double pSurvive = russianRoulette(throughput, nBounces);
        if (randFloat() > pSurvive)
            break;
        throughput /= pSurvive;
    }

    return L;
}

/// @brief Eval surface or infinite light source radiance.
/// @param scene Scene description. Used to query scene lighting condition.
/// @param itsOpt Current intersection point which could be on a light source. If there's no intersection, eval the radiance of environment light.
/// @param ray Current ray which connects last intersection point and itsOpt.
/// @return Current ray direction, obtained light radiance and solid angle dependent pdf. Note that there is no corresponding sampling process for pdf and the pdf value should NOT be applied to calculate the final radiance contribution.
PathIntegratorLocalRecord VolPathIntegrator::evalEmittance(std::shared_ptr<Scene> scene,
                                                           std::optional<Intersection> itsOpt,
                                                           const Ray &ray) {
    Vec3d wo = -ray.direction;
    Spectrum LEmission(0.0);
    double pdfDirect = 1.0;
    if (!itsOpt.has_value()) {
        auto record = evalEnvLights(scene, ray);
        LEmission = record.f;
        pdfDirect = record.pdf;
    } else if (itsOpt.value().object && itsOpt.value().object->getLight()) {
        auto its = itsOpt.value();
        Normal3d n = its.geometryNormal;
        auto light = itsOpt.value().object->getLight();
        auto record = light->eval(ray, its, ray.direction);
        LEmission = record.s;
        Intersection tmpIts;
        tmpIts.position = ray.origin;
        pdfDirect = record.pdfDirect * chooseOneLightPdf(scene, light);
    }
    return {ray.direction, LEmission, pdfDirect, false};
}

/// @brief Sample on the distribution of direct lighting and take medium transmittance into account.
/// @param scene Scene description. Multiple shadow ray intersect operations will be performed.
/// @param its Current intersection point which returned pdf dependent on.
/// @param ray Current ray. Should only be applied for time records.
/// @return Sampled direction on the distribution of direct lighting and corresponding solid angle dependent pdf. An extra flag indicites that whether it sampled on a delta distribution.
PathIntegratorLocalRecord VolPathIntegrator::sampleDirectLighting(std::shared_ptr<Scene> scene,
                                                                  const Intersection &its,
                                                                  const Ray &ray) {
    auto [light, pdfChooseLight] = chooseOneLight(scene, sampler->sample1D());
    auto record = light->sampleDirect(its, sampler->sample2D(), ray.timeMin);
    double pdfDirect = record.pdfDirect * pdfChooseLight;// pdfScatter with respect to solid angle
    Vec3d dirScatter = record.wi;
    Point3d posL = record.dst;
    Point3d posS = its.position;
    auto transmittance = evalTransmittance(scene, its, record.dst);
    //    if (!its.material && transmittance.sum() < 2.9f) {
    //        std::cout << transmittance.sum() << "\n";
    //    }
    return {dirScatter, transmittance * record.s, pdfDirect, record.isDeltaPos};
}

/// @brief Eval the scattering function, i.e., bsdf * cos or phase function. The cosine term will not be counted for delta distributed bsdf (inside f).
/// @param its Current intersection point which is used to obtain local coordinate, bxdf and phase function.
/// @param ray Current ray which is used to calculate bsdf $f(\omega_i,\omega_o)$ or phase function.
/// @param dirScatter (already sampled) scattering direction.
/// @return scattering direction, bsdf/phase value and bsdf/phase pdf.
PathIntegratorLocalRecord VolPathIntegrator::evalScatter(const Intersection &its,
                                                         const Ray &ray,
                                                         const Vec3d &dirScatter) {
    if (its.material != nullptr) {
        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
        Normal3d n = its.geometryNormal;
        double wiDotN = fm::abs(dot(n, dirScatter));
        Vec3d wi = its.toLocal(dirScatter);
        Vec3d wo = its.toLocal(-ray.direction);
        return {
            dirScatter,
            bxdf->f(wo, wi, false) * wiDotN,
            bxdf->pdf(wo, wi),
            false};
    } else {
        auto medium = its.medium;
        Vec3d wi = its.toLocal(dirScatter);
        Vec3d wo = its.toLocal(-ray.direction);
        Point3d scatteringPoint = its.position;
        auto evalPhaseResult = medium->evalPhase(wo, wi, scatteringPoint);
        return {
            dirScatter,
            std::get<0>(evalPhaseResult),// no cosine term
            std::get<1>(evalPhaseResult),
            false};
    }
}

/// @brief Sample a direction along with a pdf value in term of solid angle according to the distribution of bsdf/phase.
/// @param its Current intersection point.
/// @param ray Current incident ray.
/// @return Sampled scattering direction, bsdf * cos or phase function, corresponding pdf and whether it is sampled on a delta distribution.
PathIntegratorLocalRecord VolPathIntegrator::sampleScatter(const Intersection &its,
                                                           const Ray &ray) {
    if (its.material != nullptr) {
        Vec3d wo = its.toLocal(-ray.direction);
        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
        Vec3d n = its.geometryNormal;
        BxDFSampleResult bsdfSample = bxdf->sample(wo, sampler->sample2D(), false);
        double pdf = bsdfSample.pdf;
        Vec3d dirScatter = its.toWorld(bsdfSample.directionIn);
        double wiDotN = fm::abs(dot(dirScatter, n));
        return {dirScatter, bsdfSample.s * wiDotN, pdf, BxDF::MatchFlags(bsdfSample.bxdfSampleType, BXDF_SPECULAR)};
    } else {
        Vec3d wo = its.toLocal(-ray.direction);
        auto medium = its.medium;
        auto scatteringPoint = its.position;
        auto phaseSample = medium->samplePhase(wo, scatteringPoint, sampler->sample2D());
        Spectrum phaseValue = Spectrum(std::get<1>(phaseSample));
        double pdf = std::get<2>(phaseSample);
        Vec3d dirScatter = its.toWorld(std::get<0>(phaseSample));
        return {dirScatter, phaseValue, pdf, false};
    }
}

/// @brief Russian roulette method.
/// @param throughput Current thorughput, i.e., multiplicative (bsdf * cos) / pdf or phase / pdf.
/// @param nBounce Current bounce depth.
/// @return Survive probility after Russian roulette.
double VolPathIntegrator::russianRoulette(const Spectrum &throughput,
                                          int nBounce) {
    // double pSurvive = std::min(pRussianRoulette, throughput.sum());
    double pSurvive = pRussianRoulette;
    if (nBounce > nPathLengthLimit)
        pSurvive = 0.0;
    if (nBounce <= 2)
        pSurvive = 1.0;
    return pSurvive;
}

/// @brief (discretely) sample a light source.
/// @param scene Scene description which is used to query scene lights.
/// @param lightSample A random number within [0,1].
/// @return Pointer of the sampled light source and corresponding (discrete) probility.
std::pair<std::shared_ptr<Light>, double>
VolPathIntegrator::chooseOneLight(std::shared_ptr<Scene> scene,
                                  double lightSample) {
    // uniformly weighted
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    int numLights = lights->size();
    int lightID = std::min(numLights - 1, (int)(lightSample * numLights));
    std::shared_ptr<Light> light = lights->operator[](lightID);
    return {light, 1.0 / numLights};
}

/// @brief Calculate the (discrete) probility that a specific light source is sampled.
/// @param scene Scene description which is used to query scene lights.
/// @param light The specific light source.
/// @return Corresponding (discrete) probility.
double VolPathIntegrator::chooseOneLightPdf(std::shared_ptr<Scene> scene,
                                            std::shared_ptr<Light> light) {
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    int numLights = lights->size();
    return 1.0 / numLights;
}

/// @brief Eval the effect of environment light source (infinite area light).
/// @param scene Scene description which is used to query scene lights.
/// @param ray Current ray.
/// @return light source direction, light radiance and pdf. Without sampling process, the pdf can NOT be used to calculate final radiance.
///         Note that this function will ignore infinite medium as environment map light source should never appear together with infinite medium.
PathIntegratorLocalRecord VolPathIntegrator::evalEnvLights(std::shared_ptr<Scene> scene,
                                                           const Ray &ray) {
    std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights = scene->getLights();
    Spectrum L(0.0);
    double pdf = 0.0;
    for (auto light : *lights) {
        auto record = light->evalEnvironment(ray);
        L += record.s;
        pdf += record.pdfEmitDir;
    }
    return {-ray.direction, L, pdf};
}

/// @brief Obtain the medium while ray intersect with a surface.
/// @param its Current surface intersection point.
/// @param wi Current scattering direction.
/// @return Medium inside or outside the surface.
std::shared_ptr<Medium> VolPathIntegrator::getTargetMedium(const Intersection &its,
                                                           Vec3d wi) const {
    bool scatterToOutSide = dot(its.geometryNormal, wi) > 0;
    // bool scatterToinSide = dot(its.geometryNormal, wi) < 0;
    if (scatterToOutSide)
        return its.material->getOutsideMedium();
    else
        return its.material->getInsideMedium();

    // return nullptr;
}

/// @brief Iteratively eval the transmittance from intersection point its to direction wi.
/// @param scene Scene description used to operate ray intersection.
/// @param its Current intersection point or scattering point.
/// @param pointOnLight Sampled point on light.
/// @return The transmittance from pointOnLight to its. Transmittance will be zero if ray hits a non-null surface.
Spectrum VolPathIntegrator::evalTransmittance(std::shared_ptr<Scene> scene,
                                              const Intersection &its,
                                              Point3d pointOnLight) const {
    //if (!its.material) {
    //    std::cout << "Stop!\n";
    //}

    float tmax = (pointOnLight - its.position).length();
    Ray shadowRay{its.position, normalize(pointOnLight - its.position), 1e-4f, tmax - 1e-4f};
    std::shared_ptr<Medium> medium = its.medium;
    Spectrum tr(1.f);
    while (true) {
        auto itsOpt = scene->intersect(shadowRay);

        if (medium) {
            if (!itsOpt) {
                tr *= medium->evalTransmittance(shadowRay.origin, pointOnLight);
                break;
            }

            if (!itsOpt->material->getBxDF(*itsOpt)->isNull()) {
                tr = .0f;
                break;
            }

            tr *= medium->evalTransmittance(shadowRay.origin, itsOpt->position);
            medium = getTargetMedium(*itsOpt, shadowRay.direction);
            shadowRay.origin = itsOpt->position;
            shadowRay.timeMax -= itsOpt->t;
        } else {
            if (!itsOpt) break;

            if (!itsOpt->material->getBxDF(*itsOpt)->isNull()) {
                tr = .0f;
                break;
            }
            medium = getTargetMedium(*itsOpt, shadowRay.direction);
            shadowRay.origin = itsOpt->position;
            shadowRay.timeMax -= itsOpt->t;
        }
    }

    return tr;
}

/// @brief fulfill a specially-made Intersection representing medium scattering point.
/// @param position Scattering point position.
/// @param normal Scattering direction.
/// @param medium Current medium.
/// @return Intersection with fulfilled position, geometryNormal, shFrame and medium with material==nullptr.
Intersection VolPathIntegrator::fulfillScatteringPoint(const Point3d &position,
                                                       const Normal3d &normal,
                                                       std::shared_ptr<Medium> medium) {
    Intersection scatteringPoint;
    scatteringPoint.position = position;
    scatteringPoint.geometryNormal = -normal;
    scatteringPoint.material = nullptr;
    scatteringPoint.medium = medium;

    // medium scattering point has no shading tangent or shading bitangent.
    Vec3d pseudoShadingTangent, pseudoShadingBitangent;
    pseudoShadingTangent = normalize(cross(normal, Vec3d(0, 0, 1)));
    pseudoShadingBitangent = normalize(cross(normal, pseudoShadingTangent));
    scatteringPoint.shFrame = Frame(pseudoShadingTangent, pseudoShadingBitangent, normal);

    return scatteringPoint;
}

/// @brief Intersect in scene but ignore bsdf with isNull()==true.
/// @param scene Scene description where multiple intersect operation will be performed.
/// @param ray Initial ray.
/// @param medium Initial medium.
/// @return Intersected point and transmittance alone the way.
std::pair<std::optional<Intersection>, Spectrum>
VolPathIntegrator::intersectIgnoreSurface(std::shared_ptr<Scene> scene,
                                          const Ray &ray,
                                          std::shared_ptr<Medium> medium) const {
    const double eps = 1e-5;
    Vec3d dir = ray.direction;

    Spectrum tr(1.0);
    Ray marchRay{ray.origin + dir * eps, dir};
    std::shared_ptr<Medium> currentMedium = medium;

    Point3d lastScatteringPoint = ray.origin;
    auto testRayItsOpt = scene->intersect(marchRay);

    // calculate the transmittance of last segment from lastScatteringPoint to testRayItsOpt.
    while (true) {

        // corner case: infinite medium or infinite light source.
        if (!testRayItsOpt.has_value()) {
            if (currentMedium != nullptr)
                tr = Spectrum(0.0);
            return {testRayItsOpt, tr};
        }

        auto testRayIts = testRayItsOpt.value();

        // corner case: non-null surface
        if (testRayIts.material != nullptr) {
            if (!testRayIts.material->getBxDF(testRayIts)->isNull()) {
                if (currentMedium != nullptr)
                    tr *= currentMedium->evalTransmittance(testRayIts.position, lastScatteringPoint);
                return {testRayItsOpt, tr};
            }
        }

        // hit a null surface, calculate tr
        if (currentMedium != nullptr)
            tr *= currentMedium->evalTransmittance(testRayIts.position, lastScatteringPoint);

        // update medium
        currentMedium = getTargetMedium(testRayIts, dir);

        // update ray and intersection point.
        marchRay.origin = testRayIts.position + dir * eps;
        lastScatteringPoint = testRayIts.position;
        testRayItsOpt = scene->intersect(marchRay);
    }
    return {testRayItsOpt, tr};
}