// test testing module
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

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
#include "FunctionLayer/Light/PointLight.h"
#include "FunctionLayer/Material/MatteMaterial.h"
#include "FunctionLayer/Material/MirrorMaterial.h"
#include "FunctionLayer/Material/DielectricMaterial.h"
#include "FunctionLayer/Texture/Texture.h"
#include "FunctionLayer/Texture/ImageTexture.h"
#include "FunctionLayer/Integrator/PathIntegrator.h"
#include "FunctionLayer/Integrator/PathIntegrator.h"
#include "FunctionLayer/Shape/Triangle.h"
#include "FunctionLayer/TileGenerator/SequenceTileGenerator.h"
#include "ResourceLayer/ResourceManager.h"
#include "FunctionLayer/Shape/Quad.h"

#ifdef _WIN32
#include <direct.h>
    #define getcwd _getcwd // stupid MSFT "deprecation" warning
#else
#include "unistd.h"
#endif


TEST_CASE("load-cornell-box")
{
    Spectrum::init();
    std::cout << "NJUCG Zero v0.1" << std::endl;
    std::cout << "scene start" << std::endl;
    nlohmann::json  sceneJson;
    char dir[1024];
    getcwd(dir,sizeof(dir));
    std::filesystem::path path(dir);
    std::string rootDir= path.parent_path().parent_path().string();
    std::string sceneDir = "/scenes/scene.json";
    std::ifstream sceneFile(rootDir+sceneDir);
    sceneFile>>sceneJson;
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneJson);
   // scene->addEntity(std::make_shared<Quad>());
    std::cout << "scene created" << std::endl;
	std::cout << "building accelerator" << std::endl;
	scene->build();
    std::cout << "scene prepared" << std::endl;
    Point3d lookFrom(0, 1, 4),
        lookAt(0, 1, 0);
    Vec3d up(0, 1, 0);
    auto pinhole = std::make_shared<PinholeCamera>(
        lookFrom, lookAt, up, 35, 0.56, 3.17f);
    PathIntegrator integrator(pinhole, std::make_unique<Film>(Point2i(1000, 563), 3), std::make_unique<SequenceTileGenerator>(Point2i(1000, 563)), std::make_shared<IndependentSampler>(), 25);
    std::cout << "start rendering" << std::endl;
    integrator.render(scene);
    integrator.save("cornell-box.bmp");
    std::cout << "finish" << std::endl;
}
