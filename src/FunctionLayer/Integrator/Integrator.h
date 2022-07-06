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

#include "CoreLayer/Scene/Scene.h"
#include "FunctionLayer/Camera/Camera.h"
#include "FunctionLayer/Film/Film.h"
#include "FunctionLayer/Sampler/Sampler.h"
#include "FunctionLayer/TileGenerator.h"

#include <memory>

class Integrator
{
protected:
	std::shared_ptr<Camera> camera;
	std::unique_ptr<Film> film;
	std::unique_ptr<TileGenerator> tileGenerator;

public:
	Integrator(std::shared_ptr<Camera> camera, std::unique_ptr<Film> film, std::unique_ptr<TileGenerator> tileGenerator);
	virtual void render(std::shared_ptr<Scene> scene) = 0;
	virtual void save(const std::string& path);
	
};
