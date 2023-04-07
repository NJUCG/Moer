#include <thread>
#include "GuidedPathIntegrator.h"

GuidedPathIntegrator::GuidedPathIntegrator(std::shared_ptr<Camera> _camera,
                                           std::unique_ptr<Film> _film,
                                           std::unique_ptr<TileGenerator> _tileGenerator,
                                           std::shared_ptr<Sampler> _sampler,
                                           int _spp,
                                           int _renderThreadNum) :
    PathIntegratorNew(std::move(_camera),
                        std::move(_film),
                        std::move(_tileGenerator),
                        std::move(_sampler), _spp,
                        _renderThreadNum)
{
    totalSPP = _spp;
    isTraining = false;
    isFirstIteration = false;
    pgTree = std::make_shared<AdaptiveKDTree>();

    numFlashedSamples = 0;

#ifdef _WIN32
    Concurrency::SchedulerPolicy schedulerPolicy;
    schedulerPolicy.SetConcurrencyLimits(1, _renderThreadNum);
    Concurrency::Scheduler::SetDefaultSchedulerPolicy(schedulerPolicy);
#else
    omp_set_num_threads(_renderThreadNum);
#endif
}

void GuidedPathIntegrator::render(std::shared_ptr<Scene> scene) {
    std::vector<std::thread> threads;
    threads.reserve(renderThreadNum);

    auto tiles = tileGenerator->generateTiles();
    tilesEnd = tiles.end();

    isTraining = true;
    isFirstIteration = true;

    const int minSamplesToTrain = 128;
    int trainingSPP = trainingSPPFraction * totalSPP;

    int sppRendered = 0;
    while (sppRendered < totalSPP) {
        spp = std::min(sppPerIteration, totalSPP - sppRendered);

        numFlashedSamples = 0;
        tilesBegin = tiles.begin();

        for(int i = 0; i < renderThreadNum; ++i){
            threads.emplace_back(&GuidedPathIntegrator::renderPerThread, this, scene);
        }

        for(int i = 0; i < renderThreadNum; ++i){
            threads[i].join();
        }

        sppRendered += spp;
        threads.clear();

        if (isTraining) {
            std::cout << "got " << pgSamples.size() + numFlashedSamples << " samples" << std::endl;
            if (pgSamples.size() >= minSamplesToTrain) {
                std::cout << "start training... ";
                pgTree->update(pgSamples, renderThreadNum);
                std::cout << "training finished" << std::endl;

                pgSamples.clear();
                isFirstIteration = false;
            } else {
                std::cout << "skip training due to insufficient samples" << std::endl;
            }
        }

        std::cout << "rendered " << sppRendered << " spp" << std::endl;

        if (isTraining && sppRendered >= trainingSPP) {
            isTraining = false;
            std::vector<PGSampleData>().swap(pgSamples);
            std::cout << "stop training, rendering for the rest " << totalSPP - sppRendered << " spp" << std::endl;
        }
    }
}

void GuidedPathIntegrator::renderPerThread(const std::shared_ptr<Scene> & scene) {
    /**
   * @warning Other part of Integrator uses the original sampler
   *          so I have to fill its vectors here.
   *          In fact every time a fresh sampler is needed we should
   *          use Sampler::clone() to get one.
   */
    sampler->startPixel({0, 0});
    auto ssampler = sampler->clone(0);
    while(true)
    {
        std::unique_lock tilesLock(tilesMutex);
        if (tilesBegin == tilesEnd) {
            break;
        }
        auto tile = *(tilesBegin++);
        tilesLock.unlock();

        for(auto pixelPosition : *tile)
        {
            const auto &cam = *this->camera;
            /**
             * @bug Sampler is NOT designed for multi-threads, need copy for each thread.
             *      Sampler::clone() will return a Sampler copy, only with same sampling
             *      strategy, random numbers are not guaranteed to be identical.
             */
            // sampler->startPixel(pixelPosition);
            ssampler->startPixel(pixelPosition);
            for (int i = 0; i < spp; i++)
            {
                auto L = Li(
                    cam.generateRay(
                        film->getResolution(),
                        pixelPosition,
                        ssampler->getCameraSample()
                            ), scene
                );
                //                L = L.clamp(0.0,1.0);
                film->deposit(pixelPosition, L);
                /**
                 * @warning spp used in this for loop belongs to Integrator.
                 *          It is irrelevant with spp passed to Sampler.
                 *          And Sampler has no sanity check for subscript
                 *          of sample vector. Error may occur if spp passed to
                 *          Integrator is bigger than which passed to Sampler.
                 */
                ssampler->nextSample();
            }
        }
    }
}

Spectrum GuidedPathIntegrator::Li(const Ray &initialRay, std::shared_ptr<Scene> scene) {
    Spectrum L{.0};
    Spectrum throughput{1.0};
    Ray ray = initialRay;
    int nBounces = 0;
    auto itsOpt = scene->intersect(ray);
    PathIntegratorLocalRecord evalLightRecord = evalEmittance(scene, itsOpt, ray);

    std::vector<BounceInfo> bounceInfos;

    while (true) {

        //* All rays generated by bsdf/phase sampling which might hold radiance
        //* will be counted at the end of the loop
        //* except the ray generated by camera which is considered here
        if (!evalLightRecord.f.isBlack()) {
            L += throughput * evalLightRecord.f;
        }

        //* No intersection
        if (!itsOpt.has_value())
            break;

        auto its = itsOpt.value();

        nBounces++;
        double pSurvive = russianRoulette(throughput, nBounces);
        if (randFloat() > pSurvive)
            break;
        throughput /= pSurvive;

        bounceInfos.emplace_back();
        BounceInfo &bInfo = bounceInfos.back();
        bInfo.position = Vec3d(its.position[0], its.position[1], its.position[2]);
        bInfo.roughness = its.material->getBxDF(its)->getRoughness();

        //* ----- Direct Illumination -----
        for (int i = 0; i < nDirectLightSamples; ++i) {
            PathIntegratorLocalRecord sampleLightRecord = sampleDirectLighting(scene, its, ray);
            PathIntegratorLocalRecord evalScatterRecord = evalScatter(its, ray, sampleLightRecord.wi);

            if (!sampleLightRecord.f.isBlack()) {
                //* Multiple importance sampling
                double misw = MISWeight(sampleLightRecord.pdf, evalScatterRecord.pdf);
                if (sampleLightRecord.isDelta) {
                    misw = 1.0;
                }

                Spectrum value = sampleLightRecord.f * evalScatterRecord.f
                               / sampleLightRecord.pdf * misw
                               / nDirectLightSamples;

                L += throughput * value;
                bInfo.L += value;
            }
        }

        //*----- BSDF Sampling -----
        PathIntegratorLocalRecord sampleScatterRecord = sampleScatter(its, ray);
        if (!sampleScatterRecord.f.isBlack()) {
            throughput *= sampleScatterRecord.f / sampleScatterRecord.pdf;
        } else {
            break;
        }

        bInfo.bxdfWeight = sampleScatterRecord.f / sampleScatterRecord.pdf;
        bInfo.bxdfPdf = sampleScatterRecord.pdf;
        bInfo.direction = sampleScatterRecord.wi;
        bInfo.distanceFactor = computeDistanceFactor(its, -ray.direction, sampleScatterRecord.wi);

        //* Test whether the bsdf sampling ray hit the emitter
        const double eps = 1e-4;
        ray = Ray{its.position + sampleScatterRecord.wi * eps, sampleScatterRecord.wi};
        itsOpt = scene->intersect(ray);

        evalLightRecord = evalEmittance(scene, itsOpt, ray);
        if (!evalLightRecord.f.isBlack()) {
            //* The continuous ray hit the emitter
            //* Multiple importance sampling

            double misw = MISWeight(sampleScatterRecord.pdf, evalLightRecord.pdf);
            if (sampleScatterRecord.isDelta) {
                misw = 1.0;
            }

            Spectrum value = throughput * evalLightRecord.f * misw;
            L += value;
            bInfo.L += value;

            if (isTraining) {
                double radiance = evalLightRecord.f.average();

                if (itsOpt.has_value()) {
                    bInfo.distance = itsOpt.value().t;
                } else {
                    auto aabb = scene->getGlobalBoundingBox();
                    double radius = (aabb.pMax - aabb.pMin).length() / 2.;
                    bInfo.distance = radius * 1024;
                }

                if (bInfo.roughness >= 0.01 && radiance > 0) {
                    addPGSampleData(bInfo.position, bInfo.direction, radiance, bInfo.bxdfPdf, bInfo.distance);
                }
            }
        }

        if (itsOpt.has_value()) {
            bInfo.distance = itsOpt.value().t;
        }
    }

    if (isTraining && bounceInfos.size() >= 2) {
        double distance = bounceInfos.back().distance;
        for (int i = (int) bounceInfos.size() - 2; i >= 0; --i) {
            // compute the incident radiance
            double radiance = bounceInfos[i + 1].L.average();
            bounceInfos[i].L += bounceInfos[i + 1].L * bounceInfos[i].bxdfWeight;

            // compute the perceived distance
            if (bounceInfos[i + 1].roughness >= 0.3) {
                distance = bounceInfos[i].distance;
            } else {
                distance *= bounceInfos[i + 1].distanceFactor;
                distance += bounceInfos[i].distance;
            }

            if (bounceInfos[i].roughness >= 0.01 && radiance > 0) {
                addPGSampleData(bounceInfos[i].position, bounceInfos[i].direction, radiance, bounceInfos[i].bxdfPdf, distance);
            }
        }
    }

    return L;
}

PathIntegratorLocalRecord GuidedPathIntegrator::sampleScatter(const Intersection &its, const Ray &ray)
{
    thread_local GuidedBxDF guidedBxDF;

    Vec3d wo = its.toLocal(-ray.direction);
    std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
    Vec3d n = its.geometryNormal;

    double roughness = bxdf->getRoughness();

    bool isDelta;
    BxDFSampleResult bsdfSample;
    if (roughness < 0.01 || isFirstIteration) {
        bsdfSample = bxdf->sample(wo, sampler->sample2D(), false);
        isDelta = BxDF::MatchFlags(bsdfSample.bxdfSampleType, BXDF_SPECULAR);
    } else {
        prepareGuidedBxDF(guidedBxDF, bxdf.get(), its.position);
        bsdfSample = guidedBxDF.sample(its, wo, sampler->sample2D(), false);
        isDelta = (roughness == 0);
    }

    Vec3d dirScatter = its.toWorld(bsdfSample.directionIn);
    double wiDotN = std::abs(dot(dirScatter, n));
    return {dirScatter, bsdfSample.s * wiDotN, bsdfSample.pdf, isDelta};
}

PathIntegratorLocalRecord GuidedPathIntegrator::evalScatter(const Intersection &its,
                                                            const Ray &ray,
                                                            const Vec3d &dirScatter)
{
    thread_local GuidedBxDF guidedBxDF;

    std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);
    Normal3d n = its.geometryNormal;
    double wiDotN = std::abs(dot(n, dirScatter));
    Vec3d wi = its.toLocal(dirScatter);
    Vec3d wo = its.toLocal(-ray.direction);
    Spectrum value = bxdf->f(wo, wi, false) * wiDotN;

    double pdf;
    double roughness = bxdf->getRoughness();

    if (roughness < 0.01 || isFirstIteration) {
        pdf = bxdf->pdf(wo, wi);
    } else {
        prepareGuidedBxDF(guidedBxDF, bxdf.get(), its.position);
        pdf = guidedBxDF.pdf(its, wo, wi);
    }

    return {dirScatter, value, pdf, false};
}

double GuidedPathIntegrator::russianRoulette(const Spectrum &T,
                                             int nBounce)
{
    return nBounce > nPathLengthLimit ? 0 : 1;
}

double GuidedPathIntegrator::computeDistanceFactor(const Intersection &its,
                                                   const Vec3d &out,
                                                   const Vec3d &in)
{
    double cosThetaO = dot(its.geometryNormal, out);
    double cosThetaI = dot(its.geometryNormal, in);
    if (cosThetaO * cosThetaI <= 0) {
        double sinThetaI = std::sqrt(1 - cosThetaO * cosThetaO);
        if (sinThetaI > 0) {
            double sinThetaO = std::sqrt(1 - cosThetaI * cosThetaI);
            return sinThetaO / sinThetaI;
        }
    }
    return 1;
}

void GuidedPathIntegrator::addPGSampleData(const Vec3d &position,
                                           const Vec3d &direction,
                                           double radiance,
                                           double pdf,
                                           double distance)
{
    PGSampleData data(position, direction, radiance, pdf, distance);
    if (data.isValid() && data.distance > 1e-3) {
        std::unique_lock<std::mutex> sampleLock(pgSampleMutex);
        pgSamples.push_back(data);

        if (pgSamples.size() >= maxSampleBufferSize) {
            std::unique_lock guidingLock(pgTreeMutex);
            pgTree->update(pgSamples, renderThreadNum);
            numFlashedSamples += pgSamples.size();
            pgSamples.clear();
        }
    }
}

void GuidedPathIntegrator::prepareGuidedBxDF(GuidedBxDF &guidedBxDF,
                                             BxDF *bxdf,
                                             const Point3d &position)
{
    Vec3d p = Vec3d(position[0], position[1], position[2]);
    std::shared_lock lock(pgTreeMutex);
    pgTree->getGuidedBxDF(guidedBxDF, bxdf, p);
}
