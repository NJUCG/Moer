#include <iostream>

#include "FunctionLayer/Integrator/PathIntegrator-new.h"
#include "FunctionLayer/Integrator/NormalIntegrator.h"
#include "FunctionLayer/Integrator/VolPathIntegrator.h"
#include "FunctionLayer/Sampler/Halton.h"
#include "ResourceLayer/File/FileUtils.h"
#include "FunctionLayer/TileGenerator/SequenceTileGenerator.h"
#include "FunctionLayer/Sampler/Independent.h"
#include "FunctionLayer/Camera/CameraFactory.h"

struct RenderSettings {
    int spp;
    std::string outputPath;
    RenderSettings(const Json &json) {
        spp = getOptional(json, "spp", 32);
        outputPath = getOptional(json, "output_file", std::string("image"));
    }
};

struct TimeCounter {
    std::chrono::high_resolution_clock::time_point start, end;
    TimeCounter() {
        start = std::chrono::high_resolution_clock::now();
    }
    void Done() {
        std::chrono::time_point end = std::chrono::high_resolution_clock::now();
        auto sToken = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        std::cout << "rendering done take\n"
                  << sToken << "s";
    }
};

struct Render {
public:
    static void RenderScene(const std::string sceneWorkingDir) {
        TimeCounter renderClock;

        FileUtils::setWorkingDir(sceneWorkingDir + "/");
        Json sceneJson;
        std::ifstream sceneFile(FileUtils::getWorkingDir() + std::string("scene.json"));
        sceneFile >> sceneJson;
        std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneJson);
        std::cout << "scene created" << std::endl;
        std::cout << "building accelerator" << std::endl;
        scene->build();
        std::cout << "scene prepared" << std::endl;

        const Json &settingsJson = sceneJson["renderer"];
        settings = new RenderSettings(settingsJson);
        auto camera = CameraFactory::LoadCameraFromJson(sceneJson["camera"]);
        Point2i resolution = getOptional(sceneJson["camera"], "resolution", Point2i(512, 512));
        VolPathIntegrator integrator(camera, std::make_unique<Film>(resolution, 3),
                                     std::make_unique<SequenceTileGenerator>(resolution), std::make_shared<IndependentSampler>(settings->spp, 5), settings->spp, 12);

        std::cout << "start rendering" << std::endl;
        integrator.render(scene);
        integrator.save(settings->outputPath);
        std::cout << "finish" << std::endl;
        renderClock.Done();
    }
    static RenderSettings *settings;
};

RenderSettings *Render::settings = nullptr;

int main(int argc, const char *argv[]) {
    Spectrum::init();
    std::vector<std::string> filenames;
    for (int i = 1; i < argc; ++i) {
        Render::RenderScene(argv[i]);
    }
}