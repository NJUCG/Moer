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
#include "../src/Material/MatteMaterial.h"
#include "../src/Material/MirrorMaterial.h"
#include "../src/Material/DelectricMaterial.h"
#include "../src/Material/TestMirror.h"
#include "../../src/Integrator/PathIntegrator.h"
#include "../../src/Core/FunctionLayer/Scene.h"
#include "../../src/Core/FunctionLayer/Entity.h"
#include "../../src/Core/FunctionLayer/Camera.h"
#include "../../src/Core/FunctionLayer/Film.h"
#include "../../src/Entity/Sphere.h"
#include "../../src/Sampler/DirectSampler.h"
#include "../../src/Light/PointLight.h"

#include "../src/Integrator/PathIntegrator.h"
#include "../src/Core/FunctionLayer/Scene.h"
#include "../src/Core/FunctionLayer/Entity.h"
#include "../src/Core/FunctionLayer/Camera.h"
#include "../src/Core/FunctionLayer/Film.h"
#include "../src/Entity/Sphere.h"
#include "../src/Camera/Pinhole.h"
#include "../src/Sampler/DirectSampler.h"
#include "../src/Light/PointLight.h"

TEST_CASE("test-material-diffuse")
{
    Spectrum::init();
    std::cout << "NJUCG Zero v0.1" << std::endl;
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::cout << "scene start" << std::endl;
    std::shared_ptr<MatteMaterial> lambert = std::make_shared<MatteMaterial>(std::make_shared<ConstantTexture<Spectrum>>(RGB3(0.5, 0.5, 0.5).toSpectrum()));
    std::shared_ptr<MatteMaterial> lambertR = std::make_shared<MatteMaterial>(std::make_shared<ConstantTexture<Spectrum>>(RGB3(0.8, 0.0, 0.0).toSpectrum()));
    std::shared_ptr<MatteMaterial> lambertB = std::make_shared<MatteMaterial>(std::make_shared<ConstantTexture<Spectrum>>(RGB3(0.0, 0.0, 0.8).toSpectrum()));
    std::shared_ptr<MatteMaterial> lambertG = std::make_shared<MatteMaterial>(std::make_shared<ConstantTexture<Spectrum>>(RGB3(0.0, 0.8, 0).toSpectrum()));
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, -1.5, 1.0), 1.0, lambertG));
    scene->addEntity(std::make_shared<Sphere>(Point3d(2.1, 0.0, -2.0), 1.0, lambertR));
    scene->addEntity(std::make_shared<Sphere>(Point3d(-2.1, 0.0, -2.0), 1.0, lambertB));
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, -101.0, 0.0), 100.0, lambert));
    std::cout << "scene created" << std::endl;
    scene->addLight(std::make_shared<PointLight>(32.0, Point3d(0, 2, 1)));
    std::cout << "scene prepared" << std::endl;
//    PathIntegrator integrator(std::make_shared<TestCamera>(), std::make_unique<Film>(Point2i(128, 128), 3), nullptr, std::make_shared<DirectSampler>(), 16);
    Point3d lookFrom(0, 1, 2),
        lookAt(0, 0, 0);
    Vec3d up(0, 1, 0);
    auto pinhole = std::make_shared<PinholeCamera>(
        lookFrom, lookAt, up, 90.f, 1.f, 1.f);
    PathIntegrator integrator(pinhole, std::make_unique<Film>(Point2i(128, 128), 3), nullptr, std::make_shared<DirectSampler>(), 4);
    std::cout << "start rendering" << std::endl;
    integrator.render(scene);
    integrator.save("diffuse_result.bmp");
    std::cout << "finish" << std::endl;
    return;
}

TEST_CASE("test-material-mirror")
{
    Spectrum::init();
    std::cout << "NJUCG Zero v0.1" << std::endl;
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::cout << "scene start" << std::endl;
    std::shared_ptr<MirrorMaterial>  mirror = std::make_shared<MirrorMaterial>();
    std::shared_ptr<MatteMaterial> lambert = std::make_shared<MatteMaterial>(std::make_shared<ConstantTexture<Spectrum>>(RGB3(0.5, 0.5, 0.5).toSpectrum()));
    std::shared_ptr<MatteMaterial> lambertR = std::make_shared<MatteMaterial>(std::make_shared<ConstantTexture<Spectrum>>(RGB3(0.8, 0.0, 0.0).toSpectrum()));
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, -1.5, 1.0), 1.0, mirror));
    scene->addEntity(std::make_shared<Sphere>(Point3d(2.1, 0.0, -2.0), 1.0, lambertR));
    scene->addEntity(std::make_shared<Sphere>(Point3d(-2.1, 0.0, -2.0), 1.0, mirror));
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, -101.0, 0.0), 100.0, lambert));
    std::cout << "scene created" << std::endl;
    scene->addLight(std::make_shared<PointLight>(32.0, Point3d(0, 2, 1)));
    std::cout << "scene prepared" << std::endl;
    Point3d lookFrom(0, 1, 2),
            lookAt(0, 0, 0);
    Vec3d up(0, 1, 0);
    auto pinhole = std::make_shared<PinholeCamera>(
            lookFrom, lookAt, up, 90.f, 1.f, 1.f);
    PathIntegrator integrator(pinhole, std::make_unique<Film>(Point2i(128, 128), 3), nullptr, std::make_shared<DirectSampler>(), 4);
    std::cout << "start rendering" << std::endl;
    integrator.render(scene);
    integrator.save("mirror_result.bmp");
    std::cout << "finish" << std::endl;
    return;
}



TEST_CASE("test-material-delectric")
{
    Spectrum::init();
    std::cout << "NJUCG Zero v0.1" << std::endl;
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::cout << "scene start" << std::endl;
    std::shared_ptr<DelectricMaterial>  deletric = std::make_shared<DelectricMaterial>();
    std::shared_ptr<MirrorMaterial>  mirror = std::make_shared<MirrorMaterial>();
    std::shared_ptr<MatteMaterial> lambert = std::make_shared<MatteMaterial>(std::make_shared<ConstantTexture<Spectrum>>(RGB3(0.5, 0.5, 0.5).toSpectrum()));
    std::shared_ptr<MatteMaterial> lambertR = std::make_shared<MatteMaterial>(std::make_shared<ConstantTexture<Spectrum>>(RGB3(0.8, 0.0, 0.0).toSpectrum()));
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, -1.5, 1.0), 1.0, deletric));
    scene->addEntity(std::make_shared<Sphere>(Point3d(2.1, 0.0, -2.0), 1.0, lambertR));
    scene->addEntity(std::make_shared<Sphere>(Point3d(-2.1, 0.0, -2.0), 1.0, deletric));
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, -101.0, 0.0), 100.0, deletric));
    std::cout << "scene created" << std::endl;
    scene->addLight(std::make_shared<PointLight>(32.0, Point3d(0, 2, 1)));
    std::cout << "scene prepared" << std::endl;
    Point3d lookFrom(0, 1, 2),
            lookAt(0, 0, 0);
    Vec3d up(0, 1, 0);
    auto pinhole = std::make_shared<PinholeCamera>(
            lookFrom, lookAt, up, 90.f, 1.f, 1.f);
    PathIntegrator integrator(pinhole, std::make_unique<Film>(Point2i(128, 128), 3), nullptr, std::make_shared<DirectSampler>(), 4);    std::cout << "start rendering" << std::endl;
    integrator.render(scene);
    integrator.save("deletric_result.bmp");
    std::cout << "finish" << std::endl;
    return;
}
