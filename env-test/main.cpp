// Eigen
#include <Eigen/Core>
// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
// rapid json
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
// assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <iostream>

// test for imgui
int imgui_test();

// test for rapid json
int rapid_json_test() {
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    rapidjson::Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    rapidjson::Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    return 0;
}

// test for assimp
void LoadFinish(const aiScene* scene)
{
	std::cout << "LoadFinish ! NumVertices : " << (*(scene->mMeshes))->mNumVertices << std::endl;
}
bool LoadModel(const std::string& pFile) 
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene)
	{
		std::cout << importer.GetErrorString() << std::endl;
		return false;
	}

	// Now we can access the file's contents.
	LoadFinish(scene);

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}


int main() {
    // test for eigen
    Eigen::Vector3f v1(1, 2, 3);
    std::cout << v1 << std::endl;

    // test for stb_image
    int m_width, m_height, m_channels;
    unsigned char *m_colorMap = stbi_load(
        "..\\..\\..\\asset\\ayaka.jpg",
        &m_width,
        &m_height,
        &m_channels,
        0);
    std::cout << m_width << " " << m_height << " " << m_channels << " \n";
    
    // test for rapid json
    rapid_json_test();

    // test for assimp
    LoadModel("..\\..\\..\\asset\\WusonOBJ.obj");

    // test for imgui
    imgui_test();
}