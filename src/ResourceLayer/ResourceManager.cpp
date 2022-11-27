#include "ResourceManager.h"

#if defined(MESH_LOADER_ASSIMP)
#   include <assimp/Importer.hpp>
#   include <assimp/scene.h>
#   include <assimp/postprocess.h>
#elif defined(MESH_LOADER_TINYOBJ)
#   include <fstream>
#   include "tiny_obj_loader.h"
#else
#   error UNKNOWN MESH LOADER
#endif


// MeshDataManager implemention

std::shared_ptr<MeshDataManager> MeshDataManager::instance = nullptr;

/// @note don't need singleton pattern with mutex now. TODO
std::shared_ptr<MeshDataManager> MeshDataManager::getInstance() {
    if (!instance)
        instance.reset(new MeshDataManager());
    return instance;
}

#ifdef MESH_LOADER_TINYOBJ
namespace {

#ifdef TINYOBJLOADER_USE_DOUBLE
using TinyObjType = double;
using PositionType = double;
using NormalType = double;
using UVType = double;
using IndexType = unsigned int;

#else
using TinyObjType = float;
using PositionType = float;
using NormalType = float;
using UVType = float;
using IndexType = unsigned int;
#endif

struct TinyObjTransitionStruct {
    struct Vertex {
        std::array<PositionType, 3> position;
        std::array<NormalType, 3> normal;
        std::array<UVType, 2> uv;
    };

    struct TransitionMesh {
        std::vector<Vertex> vertices;
        std::vector<IndexType> indices;
    };

    struct TinyObjMesh {
        std::vector<std::array<PositionType, 3>> vertices;
        std::vector<std::array<NormalType, 3>> normals;
        std::vector<std::array<UVType, 2>> uvs;
        std::vector<IndexType> v_indices;
        std::vector<IndexType> vt_indices;
        std::vector<IndexType> vn_indices;

        TransitionMesh toTransitionMesh() {
            TransitionMesh mesh;
            mesh.vertices.resize(v_indices.size());
            mesh.indices.resize(v_indices.size());
            for (size_t i = 0; i < v_indices.size(); i++) {
                mesh.vertices[i].position = vertices[v_indices[i] - 1];
                mesh.indices[i] = i;
                if (vn_indices[i])
                    mesh.vertices[i].normal = normals[vn_indices[i] - 1];
                else {
                    mesh.vertices[i].normal = {(NormalType)0,
                                               (NormalType)0,
                                               (NormalType)0};
                    std::cout << "warning: vertex without normal.\n";
                }
                if (vt_indices[i])
                    mesh.vertices[i].uv = uvs[vt_indices[i] - 1];
                else {
                    mesh.vertices[i].uv = {(UVType)0,
                                           (UVType)0};
                    std::cout << "warning: vertex without texcoord.\n";
                }
            }
            return mesh;
        }
    };

    std::vector<std::pair<std::string, TinyObjMesh>> meshs;
};

void vertex_cb(void *user_data, TinyObjType x, TinyObjType y, TinyObjType z, TinyObjType w) {
    TinyObjTransitionStruct *data = reinterpret_cast<TinyObjTransitionStruct *>(user_data);
    if (data->meshs.size() == 0) {
        data->meshs.emplace_back("defaultobject", TinyObjTransitionStruct::TinyObjMesh{});
    }
    TinyObjTransitionStruct::TinyObjMesh &mesh = data->meshs.back().second;
    mesh.vertices.push_back(std::array{x, y, z});
}

void normal_cb(void *user_data, TinyObjType x, TinyObjType y, TinyObjType z) {
    TinyObjTransitionStruct *data = reinterpret_cast<TinyObjTransitionStruct *>(user_data);
    if (data->meshs.size() == 0) {
        data->meshs.emplace_back("defaultobject", TinyObjTransitionStruct::TinyObjMesh{});
    }
    TinyObjTransitionStruct::TinyObjMesh &mesh = data->meshs.back().second;
    mesh.normals.push_back(std::array{x, y, z});
}

void texcoord_cb(void *user_data, TinyObjType x, TinyObjType y, TinyObjType z) {
    TinyObjTransitionStruct *data = reinterpret_cast<TinyObjTransitionStruct *>(user_data);
    if (data->meshs.size() == 0) {
        data->meshs.emplace_back("defaultobject", TinyObjTransitionStruct::TinyObjMesh{});
    }
    TinyObjTransitionStruct::TinyObjMesh &mesh = data->meshs.back().second;
    mesh.uvs.push_back(std::array{x, y});
}

void index_cb(void *user_data, tinyobj::index_t *indices, int num_indices) {
    TinyObjTransitionStruct *data = reinterpret_cast<TinyObjTransitionStruct *>(user_data);
    if (data->meshs.size() == 0) {
        data->meshs.emplace_back("defaultobject", TinyObjTransitionStruct::TinyObjMesh{});
    }
    TinyObjTransitionStruct::TinyObjMesh &mesh = data->meshs.back().second;
    std::vector<tinyobj::index_t> tri_index;

    if (num_indices == 3) {
        tri_index = {indices[0], indices[1], indices[2]};
    } else if (num_indices == 4) {
        tri_index = {
            indices[0],
            indices[1],
            indices[2],
            indices[0],
            indices[2],
            indices[3],
        };
    }
    else {
        std::cerr << "tirangle support only.";
        std::exit(1);
    }
    for (auto &i : tri_index) {
        mesh.v_indices.push_back(i.vertex_index);
        mesh.vn_indices.push_back(i.normal_index);
        mesh.vt_indices.push_back(i.texcoord_index);
    }
}

void object_cb(void *user_data, const char *name) {
    printf("object : name = %s\n", name);
    TinyObjTransitionStruct *data = reinterpret_cast<TinyObjTransitionStruct *>(user_data);
    data->meshs.emplace_back(name, TinyObjTransitionStruct::TinyObjMesh{});
}
}// namespace
#endif// MESH_LOADER_TINYOBJ

/// @brief load and build meshes from path.
/// @param path path for a 3d model file.
/// @return unordered map for meshes. key: mesh name, value:raw mesh data.
std::shared_ptr<MeshDataCollection>
MeshDataManager::getMeshData(const std::string &path) {

    // return existing mesh data.
    auto ret=hash.find(path);
    if(ret!=hash.end()){
        return ret->second;
    }

    std::shared_ptr<MeshDataCollection> result = std::make_shared<MeshDataCollection>();

#if defined(MESH_LOADER_ASSIMP)
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
        result->operator[](std::string(ai_mesh->mName.C_Str())) = mesh_data;
    }
#elif defined(MESH_LOADER_TINYOBJ)
    tinyobj::callback_t cb;
    cb.vertex_cb = vertex_cb;
    cb.normal_cb = normal_cb;
    cb.texcoord_cb = texcoord_cb;
    cb.index_cb = index_cb;
    //cb.usemtl_cb = usemtl_cb;
    //cb.mtllib_cb = mtllib_cb;
    //cb.group_cb = group_cb;
    cb.object_cb = object_cb;

    TinyObjTransitionStruct transition_data;
    std::string warn;
    std::string err;
    std::string filename = path;
    std::ifstream ifs(path.c_str());

    if (ifs.fail()) {
        std::cerr << "file not found." << std::endl;
        std::exit(1);
    }

    std::cout << "Parsing " << path << std::endl;

    bool load_flag = tinyobj::LoadObjWithCallback(ifs, cb, &transition_data, NULL, &warn, &err);

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!load_flag) {
        std::cerr << "Failed to parse .obj" << std::endl;
        std::exit(1);
    }

    std::cout << transition_data.meshs.size() << " meshes totally\n";

    for (auto &[name, tiny_mesh] : transition_data.meshs) {
        std::shared_ptr<MeshData> mesh_data = std::make_shared<MeshData>();
        auto mesh = tiny_mesh.toTransitionMesh();
        //*---------------------------------------------
        //*-----------  Parsing vertices  --------------
        //*---------------------------------------------
        if (!mesh.vertices.size()) {
            std::cerr << "Mesh without vertices is not supported \n";
            std::exit(1);
        }
        mesh_data->m_vertices.resize(3, mesh.vertices.size());
        mesh_data->m_normals.resize(3, mesh.vertices.size());
        mesh_data->m_UVs.resize(mesh.vertices.size());
        for (size_t i = 0; i < mesh.vertices.size(); i++) {
            mesh_data->m_vertices(0, i) = mesh.vertices[i].position[0];
            mesh_data->m_vertices(1, i) = mesh.vertices[i].position[1];
            mesh_data->m_vertices(2, i) = mesh.vertices[i].position[2];

            mesh_data->m_normals(0, i) = mesh.vertices[i].normal[0];
            mesh_data->m_normals(1, i) = mesh.vertices[i].normal[1];
            mesh_data->m_normals(2, i) = mesh.vertices[i].normal[2];

            mesh_data->m_UVs[i] = Point2d{mesh.vertices[i].uv[0], mesh.vertices[i].uv[1]};
        }

        // indices
        mesh_data->m_indices.reserve(mesh.indices.size() / 3);
        for (size_t j = 0; j < mesh.indices.size() / 3; ++j) {
            mesh_data->m_indices.emplace_back(
                static_cast<int>(mesh.indices[j * 3 + 0]),
                static_cast<int>(mesh.indices[j * 3 + 1]),
                static_cast<int>(mesh.indices[j * 3 + 2]));
        }

        result->insert(std::make_pair(name, mesh_data));
    }
#endif
    hash[path]=result;
    
    return result;
}

// ImageManager implemention

std::shared_ptr<ImageManager> ImageManager::instance=nullptr;

std::shared_ptr<ImageManager>
ImageManager::getInstance(){
    if(!instance)
        instance.reset(new ImageManager());
    return instance;
}

std::shared_ptr<Image> ImageManager::getImage(const std::string &path, Image::ImageLoadMode mode){
    auto ret=hash.find(path);
    if(ret!=hash.end()){
        return ret->second;
    }

    Image* img=new Image(path,mode);
    std::shared_ptr<Image> imgPtr(img);
    hash[path]=imgPtr;
    return imgPtr;
}