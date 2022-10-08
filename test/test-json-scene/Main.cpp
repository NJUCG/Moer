// test testing module
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include <filesystem>
#include <iostream>
#include <cstdio>
#include <vector>
#include <memory>
#include "CoreLayer/Scene/Scene.h"
#include "FunctionLayer/Camera/Camera.h"
#include "FunctionLayer/Camera/Thinlens.h"
#include "FunctionLayer/Camera/Pinhole.h"
#include "FunctionLayer/Film/Film.h"
#include "FunctionLayer/Shape/Entity.h"
#include "FunctionLayer/Sampler/DirectSampler.h"
#include "FunctionLayer/Sampler/Stratified.h"
#include "FunctionLayer/Sampler/Independent.h"

#include "FunctionLayer/Texture/Texture.h"
#include "FunctionLayer/Integrator/NormalIntegrator.h"
#include "FunctionLayer/Integrator/VolPathIntegrator.h"
#include "FunctionLayer/TileGenerator/SequenceTileGenerator.h"
#include "ResourceLayer/ResourceManager.h"
#include "ResourceLayer/File/FileUtils.h"


#ifdef _WIN32
#include <direct.h>
    #define getcwd _getcwd // stupid MSFT "deprecation" warning
#else
#include "unistd.h"
#include "FunctionLayer/Integrator/PathIntegrator.h"
#include "FunctionLayer/Integrator/PathIntegrator-new.h"

#endif


TEST_CASE("load-cornell-box")
{
    Spectrum::init();

    std::cout << "NJUCG Zero v0.1" << std::endl;
    std::cout << "scene start" << std::endl;
    Json  sceneJson;

    std::ifstream sceneFile(FileUtils::WorkingDir+"scene.json");
    sceneFile>>sceneJson;
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneJson);
   // scene->addEntity(std::make_shared<Quad>());
    std::cout << "scene created" << std::endl;
	std::cout << "building accelerator" << std::endl;
	scene->build();
    std::cout << "scene prepared" << std::endl;
    Point3d lookFrom(0, 1, 6.8),
        lookAt(0, 1, 0);
    Vec3d up(0, 1, 0);
    auto pinhole = std::make_shared<PinholeCamera>(
        lookFrom, lookAt, up, 35, 1/0.56, 3.17f);
    PathIntegrator integrator(pinhole, std::make_unique<Film>(Point2i(1000, 563), 3), std::make_unique<SequenceTileGenerator>(Point2i(1000, 563)), std::make_shared<IndependentSampler>(), 16, 12);
//    NormalIntegrator integrator(pinhole, std::make_unique<Film>(Point2i(1000, 563), 3), std::make_unique<SequenceTileGenerator>(Point2i(1000, 563)), std::make_shared<IndependentSampler>(), 16, 12);

    std::cout << "start rendering" << std::endl;
    integrator.render(scene);
    integrator.save("cornell-final-final-path-new.bmp");
    std::cout << "finish" << std::endl;
}


/*
TEST_CASE("test-ball")
{
    Spectrum::init();
    std::cout << "NJUCG Zero v0.1" << std::endl;
    std::cout << "scene start" << std::endl;
    Json  sceneJson;

    std::ifstream sceneFile(FileUtils::WorkingDir+"scene.json");
    sceneFile>>sceneJson;
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneJson);
    // scene->addEntity(std::make_shared<Quad>());
    std::cout << "scene created" << std::endl;
    std::cout << "building accelerator" << std::endl;
    scene->build();
    std::cout << "scene prepared" << std::endl;
    Point3d lookFrom(3.04068,
                     3.17153,
                     3.20454),
            lookAt(0.118789,
                   0.473398,
                   0.161081);
    Vec3d up(0, 1, 0);
    auto pinhole = std::make_shared<PinholeCamera>(
            lookFrom, lookAt, up, 35, (float(1280)/float(780)), 3.17f);

    PathIntegratorNew integrator(pinhole, std::make_unique<Film>(Point2i(1280,780), 3),
            std::make_unique<SequenceTileGenerator>(Point2i(1280,780)), std::make_shared<IndependentSampler>(), 32, 12);
    std::cout << "start rendering" << std::endl;
    integrator.render(scene);
    integrator.save("testball.bmp");
    std::cout << "finish" << std::endl;
}
*/