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
#include "FunctionLayer/Shape/Sphere.h"
#include "FunctionLayer/Sampler/DirectSampler.h"
#include "FunctionLayer/Sampler/Independent.h"
#include "FunctionLayer/Light/PointLight.h"
#include "FunctionLayer/Light/DiffuseAreaLight.h"
#include "FunctionLayer/Material/MatteMaterial.h"
#include "FunctionLayer/Material/TestMirror.h"
#include "FunctionLayer/Texture/Texture.h"
#include "FunctionLayer/Texture/ImageTexture.h"
#include "FunctionLayer/Integrator/PathIntegrator.h"
#include "FunctionLayer/Integrator/PathIntegrator.h"


TEST_CASE("test-integrator-validate")
{
    Spectrum::init();
    std::cout << "NJUCG Zero v0.1" << std::endl;
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::cout << "scene start" << std::endl;
    std::shared_ptr<MatteMaterial> lambert = std::make_shared<MatteMaterial>(std::make_shared<ConstantTexture<Spectrum>>(RGB3(0.5, 0.5, 0.5).toSpectrum()));
    std::shared_ptr<TestMirror> mirror = std::make_shared<TestMirror>();
    auto sphereEmitterShape = std::make_shared<Sphere>(Point3d(-2, 2, -3), 0.5, lambert);
    auto sphereEmitterLight = std::make_shared<DiffuseAreaLight>(sphereEmitterShape, 50.0);
    sphereEmitterShape->setLight(sphereEmitterLight);
    scene->addEntity(sphereEmitterShape);
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, 0.0, -1.0), 1.0, lambert));
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, -101.0, 0.0), 100.0, lambert));
    std::cout << "scene created" << std::endl;
    scene->addLight(std::make_shared<PointLight>(20.0, Point3d(2, 1, -3)));
    scene->addLight(sphereEmitterLight);
    std::cout << "scene prepared" << std::endl;

    Point3d lookFrom(0, 2, 3),
        lookAt(0, 0, 0);
    Vec3d up(0, 1, 0);
    auto pinhole = std::make_shared<PinholeCamera>(
        lookFrom, lookAt, up, 90.f, 1.f, 1.f);
    auto thinlens = std::make_shared<ThinlensCamera>(
        lookFrom, lookAt, up, 90.f, 1.f, 2.2, 0.50);

    PathIntegrator integrator(pinhole, std::make_unique<Film>(Point2i(128, 128), 3), nullptr, std::make_shared<IndependentSampler>(), 64);
    std::cout << "start rendering" << std::endl;
    integrator.render(scene);
    integrator.save("result-integrator-validate.bmp");
    std::cout << "finish" << std::endl;
    return;
}