/**
 * @file Integrator.cpp
 * @author Zhimin Fan
 * @brief Integrator
 * @version 0.1
 * @date 2022-05-31
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../Core/FunctionLayer/Integrator.h"

Integrator::Integrator(std::shared_ptr<Camera> camera, std::unique_ptr<Film> film, std::unique_ptr<TileGenerator> tileGenerator) : camera(camera),
                                                                                                                                   film(std::move(film)),
                                                                                                                                   tileGenerator(std::move(tileGenerator))
{
}

void Integrator::save(const std::string &path)
{
    film->save(path);
}
