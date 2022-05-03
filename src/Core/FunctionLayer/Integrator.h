/**
 * @file Integrator.h
 * @author orbitchen
 * @brief Renderer. The start of everything.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Scene.h"
#include "Camera.h"
#include "Film.h"
#include "TileGenerator.h"
#include "Sampler.h"

#include <memory>

class Integrator
{
protected:
	std::shared_ptr<Camera> camera;
	std::unique_ptr<Film> film;
	std::unique_ptr<TileGenerator> tileGenerator;

public:
	void render(std::shared_ptr<Scene> scene) const = 0;
};

class SampleIntegrator : public Integrator
{
protected:
	std::shared_ptr<Sampler> sampler;

public:
	void render(std::shared_ptr<Scene> scene) const = 0;
};