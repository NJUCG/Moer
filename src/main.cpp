#include <iostream>
#include <fstream>
#include "FunctionLayer/Integrator/PathIntegrator-new.h"
#include "ResourceLayer/File/FileUtils.h"
#include "FunctionLayer/TileGenerator/SequenceTileGenerator.h"
#include "FunctionLayer/Sampler/Independent.h"
#include "FunctionLayer/Camera/CameraFactory.h"
#include "FunctionLayer/Camera/Pinhole.h"


struct RenderSettings{
    int spp;
    std::string outputPath;
    RenderSettings(const Json & json){
        spp= getOptional(json,"spp",32);
        outputPath = getOptional(json,"output_file",std::string("image"));
    }
};

struct TimeCounter{
    std::chrono::high_resolution_clock::time_point start,end;
    TimeCounter(){
        start =  std::chrono::high_resolution_clock::now();
    }
    void Done(){
        std::chrono::time_point end =  std::chrono::high_resolution_clock::now();
        auto sToken =std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        std::cout<<"rendering done take\n"<<sToken<<"s";
    }
};

struct Render{
public:
   static void RenderScene(const std::string sceneWorkingDir){
       TimeCounter renderClock;

       FileUtils::setWorkingDir(sceneWorkingDir + "/");
       Json sceneJson;
       std::ifstream sceneFile(FileUtils::getWorkingDir()+std::string("scene.json"));
       sceneFile>>sceneJson;
       std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneJson);
       std::cout << "scene created" << std::endl;
       std::cout << "building accelerator" << std::endl;
       scene->build();
       std::cout << "scene prepared" << std::endl;

       const Json & settingsJson = sceneJson["renderer"];
       settings = new RenderSettings(settingsJson);
        //    auto camera = CameraFactory::LoadCameraFromJson(sceneJson["camera"]);
       auto camera = std::make_shared<PinholeCamera>(Point3d(0,1,5),Point3d(0,1,0),Vec3d(0,1,0),30,1.0,1.0);
       Point2i resolution = getOptional(sceneJson["camera"],"resolution",Point2i(256,256));
       PathIntegratorNew integrator(camera, std::make_unique<Film>(resolution, 3),
                                    std::make_unique<SequenceTileGenerator>(resolution), std::make_shared<IndependentSampler>(), 4, 16);
       std::cout << "start rendering" << std::endl;
       integrator.render(scene);
       integrator.save("J:/Moer/scenes/cornell-box/output");
       std::cout << "finish" << std::endl;
       renderClock.Done();
   }
    static RenderSettings *  settings;
};

RenderSettings *  Render::settings = nullptr;

int main(int argc, const char *argv[]){
    std::cout<<"Moer begin\n";
    Spectrum::init();
    Render::RenderScene("J:/Moer/scenes/cornell-box");
}