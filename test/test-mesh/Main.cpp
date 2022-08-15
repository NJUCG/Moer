#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "ResourceLayer/File/MeshData.h"
#include "ResourceLayer/ResourceManager.h"

TEST_CASE("test-mesh")
{
    auto meshDataManager = MeshDataManager::getInstance();

    std::vector<std::shared_ptr<MeshData>> meshes 
        = meshDataManager->getMeshData("/mnt/renderer/Zero/asset/bunny.obj");
    return;
    std::cout << "mMeshes = " << meshes.size() << std::endl;
    for (auto itr : meshes) {
        std::cout << "mFaces = " << itr->getTriangleNum() << std::endl;
    }
}