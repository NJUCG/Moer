#include "FunctionLayer/Integrator/PathIntegrator-new.h"
#include "ResourceLayer/File/FileUtils.h"
#include "FunctionLayer/Camera/Pinhole.h"
#include "FunctionLayer/TileGenerator/SequenceTileGenerator.h"
#include "FunctionLayer/Sampler/Independent.h"

#include <iostream>
#include <fstream>

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
       //todo : config camera integrator,film,tile Generator,sampler by Json
       Point3d lookFrom(3.04068,
                        3.17153,
                        3.20454),
               lookAt(0.118789,
                      0.473398,
                      0.161081);
       Vec3d up(0, 1, 0);
       auto pinhole = std::make_shared<PinholeCamera>(
               lookFrom, lookAt, up, 35, (float(1280)/float(780)), 3.17f);

       PathIntegratorNew integrator(pinhole, std::make_unique<Film>(Point2i(1280,780), 3),
                                    std::make_unique<SequenceTileGenerator>(Point2i(1280,780)), std::make_shared<IndependentSampler>(), 32, 12);
       std::cout << "start rendering" << std::endl;
       integrator.render(scene);
       integrator.save("testball");
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