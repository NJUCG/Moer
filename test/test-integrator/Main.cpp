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
#include "../src/Material/TestMaterialLambert.h"

int main()
{
    Spectrum::init();
    std::cout << "NJUCG Zero v0.1" << std::endl;
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::cout << "scene start" << std::endl;
    std::shared_ptr<TestMaterialLambert> lambert = std::make_shared<TestMaterialLambert>();
    std::shared_ptr<TestMaterialLambert> lambertR = std::make_shared<TestMaterialLambert>(RGB3(0.3, 0.0, 0.0).toSpectrum());
    std::shared_ptr<TestMaterialLambert> lambertG = std::make_shared<TestMaterialLambert>(RGB3(0.0, 0.3, 0.0).toSpectrum());
    std::shared_ptr<TestMaterialLambert> lambertB = std::make_shared<TestMaterialLambert>(RGB3(0.0, 0.0, 0.3).toSpectrum());
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, 0.0, -2.0), 1.0, lambertG));
    scene->addEntity(std::make_shared<Sphere>(Point3d(2.1, 0.0, -2.0), 1.0, lambertR));
    scene->addEntity(std::make_shared<Sphere>(Point3d(-2.1, 0.0, -2.0), 1.0, lambertB));
    scene->addEntity(std::make_shared<Sphere>(Point3d(0.0, -101.0, 0.0), 100.0, lambert));
    std::cout << "scene created" << std::endl;
    scene->addLight(std::make_shared<PointLight>(24.0, Point3d(0, 2, 1)));
    std::cout << "scene prepared" << std::endl;
    PathIntegrator integrator(std::make_shared<TestCamera>(), std::make_unique<Film>(Point2i(128, 128), 3), nullptr, std::make_shared<DirectSampler>(), 16);
    std::cout << "start rendering" << std::endl;
    integrator.render(scene);
    integrator.save("result.bmp");
    std::cout << "finish" << std::endl;
    return 0;
}