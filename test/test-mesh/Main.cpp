#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ResourceLayer/File/MeshData.h"
#include "ResourceLayer/ResourceManager.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <memory>
#include <chrono>
#include "CoreLayer/Scene/Scene.h"
#include "FunctionLayer/Camera/Camera.h"
#include "FunctionLayer/Camera/Thinlens.h"
#include "FunctionLayer/Camera/Pinhole.h"
#include "FunctionLayer/Film/Film.h"
#include "FunctionLayer/Shape/Entity.h"
#include "FunctionLayer/Shape/Sphere.h"
#include "FunctionLayer/Shape/Mesh.h"
#include "FunctionLayer/Sampler/DirectSampler.h"
#include "FunctionLayer/Sampler/Stratified.h"
#include "FunctionLayer/Sampler/Independent.h"
#include "FunctionLayer/Light/PointLight.h"
#include "FunctionLayer/Material/MatteMaterial.h"
#include "FunctionLayer/Material/MirrorMaterial.h"
#include "FunctionLayer/Material/DelectricMaterial.h"
#include "FunctionLayer/Texture/Texture.h"
#include "FunctionLayer/Texture/ImageTexture.h"
#include "FunctionLayer/Integrator/PathIntegrator.h"
#include "FunctionLayer/Integrator/PathIntegrator.h"
#include "FunctionLayer/Shape/Triangle.h"
#include "FunctionLayer/TileGenerator/SequenceTileGenerator.h"
#include "ResourceLayer/ResourceManager.h"


TEST_CASE("test-mesh")
{
    auto meshDataManager = MeshDataManager::getInstance();

    std::vector<std::shared_ptr<MeshData>> meshes 
        = meshDataManager->getMeshData("../../../asset/monkey.obj");
    

    Spectrum::init();
    std::cout << "NJUCG Zero v0.1" << std::endl;
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::cout << "scene start" << std::endl;
    std::shared_ptr<MatteMaterial> lambert = std::make_shared<MatteMaterial>(std::make_shared<ConstantTexture<Spectrum>>(RGB3(0.5, 0.5, 0.5).toSpectrum()));

    for (auto data : meshes) {
        scene->addEntity(std::make_shared<Mesh>(data, lambert));
    }
    
    scene->addLight(std::make_shared<PointLight>(32.0, Point3d(0, 0, 1)));

    scene->build();

    Point3d lookFrom(0, 0, 0.3),
        lookAt(0, 0, 0);
    Vec3d up(0, 1, 0);
    auto pinhole = std::make_shared<PinholeCamera>(
        lookFrom, lookAt, up, 90.f, 1.f, 1.f);
    PathIntegrator integrator(pinhole, std::make_unique<Film>(Point2i(128 * 3, 128 * 3), 3), std::make_unique<SequenceTileGenerator>(Point2i(128 * 3, 128 * 3)), std::make_shared<IndependentSampler>(), 1, 12);
    std::cout << "start rendering" << std::endl;
    auto before = std::chrono::steady_clock::now();
    integrator.render(scene);
    integrator.save("mesh-new.bmp");
    std::cout << "finish" << std::endl;
    auto cost = std::chrono::steady_clock::now() - before;
    std::cout << (float)std::chrono::duration_cast<std::chrono::milliseconds>(cost).count() / 1000.f << std::endl;
}