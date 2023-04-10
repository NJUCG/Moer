/**
 * @file Integrator.h
 * @author orbitchen
 * @brief Renderer. The start of everything.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <memory>

#include "FunctionLayer/Scene/Scene.h"
#include "FunctionLayer/Camera/Camera.h"
#include "FunctionLayer/Film/Film.h"
#include "FunctionLayer/Sampler/Sampler.h"
#include "FunctionLayer/TileGenerator/TileGenerator.h"

/// @defgroup Integrator

/**
 * @brief Base class for all integrators solving the rendering equation
 * @ingroup Integrator
 */
class Integrator {
protected:
    std::shared_ptr<Camera> camera;
    std::unique_ptr<Film> film;

public:
    /**
	 * @brief Construct a new Integrator object
	 * 
	 * @param _camera
	 * @param _film 
	 */
    Integrator(std::shared_ptr<Camera> _camera, std::unique_ptr<Film> _film);
    /**
	 * @brief Start to render the scene
	 * 
	 * @param scene The scene to be rendered
	 */
    virtual void render(std::shared_ptr<Scene> scene) = 0;
    /**
	 * @brief Save the rendering result
	 * 
	 * @param path The path to save the result
	 */
    virtual void save(const std::string &path);
};

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

inline void printProgress(float percentage) {
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}
