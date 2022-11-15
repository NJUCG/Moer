#include <iostream>
#include <fstream>
#include "FunctionLayer/Integrator/PathIntegrator-new.h"
#include "ResourceLayer/File/FileUtils.h"
#include "FunctionLayer/TileGenerator/SequenceTileGenerator.h"
#include "FunctionLayer/Sampler/Independent.h"
#include "FunctionLayer/Camera/CameraFactory.h"


struct RenderSettings{
    int spp;
    std::string outputPath;
    RenderSettings(const Json & json){
        spp= getOptional(json,"spp",32);
        outputPath = getOptional(json,"output_file",std::string("image"));
    }
};

struct Render{
public:
   static  void RenderScene(const std::string sceneWorkingDir){
       FileUtils::setWorkingDir(sceneWorkingDir + "/");
       Json  sceneJson;
       std::ifstream sceneFile(FileUtils::getWorkingDir()+std::string("scene.json"));
       sceneFile>>sceneJson;
       std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneJson);
       std::cout << "scene created" << std::endl;
       std::cout << "building accelerator" << std::endl;
       scene->build();
       std::cout << "scene prepared" << std::endl;

       const Json & settingsJson = sceneJson["renderer"];
       settings = new RenderSettings(settingsJson);
       auto camera = CameraFactory::LoadCameraFromJson(sceneJson["camera"]);
       Point2i resolution = getOptional(sceneJson["camera"],"resolution",Point2i(512,512));
       PathIntegratorNew integrator(camera, std::make_unique<Film>(resolution, 3),
                                    std::make_unique<SequenceTileGenerator>(resolution), std::make_shared<IndependentSampler>(), settings->spp, 12);
       std::cout << "start rendering" << std::endl;
       integrator.render(scene);
       integrator.save(settings->outputPath);
       std::cout << "finish" << std::endl;
   }
    static RenderSettings *  settings;
};

RenderSettings *  Render::settings = nullptr;

int main(int argc, const char *argv[]){
    Spectrum::init();
    std::vector<std::string> filenames;
    for (int i = 1; i < argc; ++i) {
        Render::RenderScene(argv[i]);
    }
}