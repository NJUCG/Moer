// test testing module
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "../../src/Texture/ProceduralTexture.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include "../src/Integrator/PathIntegrator.h"
#include "../src/Core/FunctionLayer/Scene.h"
#include "../src/Core/FunctionLayer/Entity.h"
#include "../src/Core/FunctionLayer/Camera.h"
#include "../src/Core/FunctionLayer/Film.h"
#include "../src/Entity/Sphere.h"
#include "../src/Camera/TestCamera.h"
#include "../src/Sampler/DirectSampler.h"
#include "../src/Light/PointLight.h"
#include "../src/Material/Matte.h"

TEST_CASE("test-material")
{
    Spectrum::init();
    std::cout << "NJUCG Zero v0.1" << std::endl;
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::cout << "scene start" << std::endl;
    std::shared_ptr<Matte> matte  = std::make_shared<Matte>();
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, 0.0, -2.0), 1.0,  matte));
    // scene->addEntity(std::make_shared<Sphere>(Point3d(2.1, 0.0, -1.5), 1.0, lambert));
    // scene->addEntity(std::make_shared<Sphere>(Point3d(-2.1, 0.0, -1.5), 1.0, lambert));
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, -101.0, 0.0), 100.0, matte));
    std::cout << "scene created" << std::endl;
    scene->addLight(std::make_shared<PointLight>(32.0, Point3d(0, 2, 1)));
    std::cout << "scene prepared" << std::endl;
    PathIntegrator integrator(std::make_shared<TestCamera>(), std::make_unique<Film>(Point2i(128, 128), 3), nullptr, std::make_shared<DirectSampler>(), 4);
    std::cout << "start rendering" << std::endl;
    integrator.render(scene);
    integrator.save("diffuse_result.bmp");
    std::cout << "finish" << std::endl;
    return;
}