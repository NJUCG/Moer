#include <iostream>
#include <fstream>
#include "FunctionLayer/Integrator/PathIntegrator-new.h"
#include "ResourceLayer/File/FileUtils.h"
#include "FunctionLayer/Camera/Pinhole.h"
#include "FunctionLayer/TileGenerator/SequenceTileGenerator.h"
#include "FunctionLayer/Sampler/Independent.h"
#include "FunctionLayer/Sampler/Stratified.h"
#include "FunctionLayer/Sampler/Halton.h"

struct Render{
public:
   static  void RenderScene(const std::string sceneWorkingDir){
       FileUtils::setWorkingDir(sceneWorkingDir + "/");
       Json sceneJson;
       std::ifstream sceneFile(FileUtils::getWorkingDir()+std::string("scene.json"));
       sceneFile>>sceneJson;
       std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneJson);
       std::cout << "scene created" << std::endl;
       std::cout << "building accelerator" << std::endl;
       scene->build();
       std::cout << "scene prepared" << std::endl;
       //todo : config camera integrator,film,tile Generator,sampler by Json
       /// @note Do we need reflection in order to configure them in GUI?
       int res_w = 600, res_h = 600;
       Point3d lookFrom(0, 1, 4.5),
               lookAt(0, 1, 0);
       Vec3d up(0, 1, 0);
       auto pinhole = std::make_shared<PinholeCamera>(
               lookFrom, lookAt, up, 35, (float(res_w)/float(res_h)), 3.17f);

       PathIntegratorNew integrator(
        pinhole,                                                    // Camera
        std::make_unique<Film>(Point2i(res_w, res_h), 3),               // Film
        std::make_unique<SequenceTileGenerator>(Point2i(res_w, res_h)), // Tile
        // std::make_shared<IndependentSampler>(),                     // Sampler
        // std::make_shared<StratifiedSampler>(6, 10),
        std::make_shared<HaltonSampler>(6, res_w, res_h, 10),
        16,                                                         // Spp
        12                                                          // nThread
        );
       std::cout << "start rendering" << std::endl;
       integrator.render(scene);
       integrator.save("cornell-box.halton");
       std::cout << "finish" << std::endl;
   }
};

int main(int argc, const char *argv[]){
    Spectrum::init();
    std::vector<std::string> filenames;
    for (int i = 1; i < argc; ++i) {
        Render::RenderScene(argv[i]);
    }
}