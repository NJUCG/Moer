#include "ResourceManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::shared_ptr<MeshDataManager> MeshDataManager::instance = nullptr;

std::shared_ptr<MeshDataManager> 
MeshDataManager::getInstance() {
    if (!instance)
        instance.reset(new MeshDataManager());
    return instance;
}

std::vector<std::shared_ptr<MeshData>>
MeshDataManager::getMeshData(const std::string &path) {
    std::vector<std::shared_ptr<MeshData>> result;
    
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile (
        path, 
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_JoinIdenticalVertices
    );

    if (nullptr == scene) {
        std::cerr << "Error when parseing the mesh file\n";
        std::exit(1);
    }

    std::cout << "Parsing " << path << std::endl;

    std::cout << scene->mNumMeshes << " meshes totally\n";

    
    for (int i = 0; i < scene->mNumMeshes; ++i) {
        const auto ai_mesh = scene->mMeshes[i];
        std::cout << ai_mesh->mName.C_Str() << std::endl;
        
        std::shared_ptr<MeshData> mesh_data;
        mesh_data.reset(new MeshData());
        
        //*---------------------------------------------
        //*-----------  Parsing vertices  --------------
        //*---------------------------------------------

        if (!ai_mesh->HasPositions()) {
            std::cerr << "Mesh without vertices is not supported \n";
            std::exit(1);
        }

        mesh_data->m_vertices.resize(3, ai_mesh->mNumVertices);
        std::memcpy(
            mesh_data->m_vertices.data(),
            ai_mesh->mVertices,
            sizeof(aiVector3D) * ai_mesh->mNumVertices
        );

        //*---------------------------------------------
        //*-----------  Parsing normals  --------------
        //*---------------------------------------------

        if (!ai_mesh->HasNormals()) {
            std::cerr << "Mesh without normals is not supported \n";
            std::exit(1);
        }

        mesh_data->m_normals.resize(3, ai_mesh->mNumVertices);
        std::memcpy(
            mesh_data->m_normals.data(),
            ai_mesh->mNormals,
            sizeof(aiVector3D) * ai_mesh->mNumVertices
        );

        //*---------------------------------------------
        //*-----------  Parsing UVs  --------------
        //*---------------------------------------------
        if (ai_mesh->GetNumUVChannels() == 1 && ai_mesh->mNumUVComponents[0] == 2) {
            mesh_data->m_UVs.reserve(ai_mesh->mNumVertices);
            auto text_coords = ai_mesh->mTextureCoords[0];
            for (int j = 0; j < ai_mesh->mNumVertices; ++j) {
            
                mesh_data->m_UVs.emplace_back(
                    Point2d {
                        text_coords[j][0],
                        text_coords[j][1]
                    }
                );
            }   
        } else {
            std::cerr << "Mesh without uv coordinates (or not 2d) is not supported \n";
            std::exit(1);
        }


        //*---------------------------------------------
        //*-----------  Parsing indices  --------------
        //*---------------------------------------------
        //! Only triangle mesh support
        //! And there is a convention from unsigned to int, might crush

        if (!ai_mesh->HasFaces()) {
            std::cerr << "Mesh without indices is not supported \n";
            std::exit(1);
        }

        mesh_data->m_indices.reserve(ai_mesh->mNumFaces);
        for (int j = 0; j < ai_mesh->mNumFaces; ++j) {
            aiFace indices = ai_mesh->mFaces[j];
            mesh_data->m_indices.emplace_back(
                Point3i {
                    static_cast<int>(indices.mIndices[0]),
                    static_cast<int>(indices.mIndices[1]),
                    static_cast<int>(indices.mIndices[2])
                }
            );
        }

        //*--------------------------------------
        //*--------- Parsing tangent  -----------
        //*--------------------------------------
        if (!ai_mesh->HasTangentsAndBitangents()) {
            std::cerr << "Mesh without tangent-space is not supported \n!";
            std::exit(1);
        }
        mesh_data->m_tangents.resize(3, ai_mesh->mNumVertices);
        std::memcpy(
            mesh_data->m_tangents.data(), 
            ai_mesh->mTangents, 
            sizeof(aiVector3D) * ai_mesh->mNumVertices
        );
        
        mesh_data->m_bitangents.resize(3, ai_mesh->mNumVertices);
        std::memcpy(
            mesh_data->m_bitangents.data(), 
            ai_mesh->mBitangents, 
            sizeof(aiVector3D) * ai_mesh->mNumVertices
        );

        //*--------------------------------------
        //*------------ Parsing AABB ------------
        //*--------------------------------------
        //! Some problems in assimp aabb

        double minX = mesh_data->m_vertices.row(0).minCoeff(),
               maxX = mesh_data->m_vertices.row(0).maxCoeff(),
               minY = mesh_data->m_vertices.row(1).minCoeff(),
               maxY = mesh_data->m_vertices.row(1).maxCoeff(),
               minZ = mesh_data->m_vertices.row(2).minCoeff(),
               maxZ = mesh_data->m_vertices.row(2).maxCoeff();

        mesh_data->m_aabb = BoundingBox3f(
            Point3d {minX, minY, minZ},
            Point3d {maxX, maxY, maxZ}
        );

        result.emplace_back(mesh_data);
    }

    return result;

}