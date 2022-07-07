/**
 * @file Integrator.cpp
 * @author Zhimin Fan
 * @brief Integrator
 * @version 0.1
 * @date 2022-05-31
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "Integrator.h"

Integrator::Integrator(
    std::shared_ptr<Camera> _camera, 
    std::unique_ptr<Film> _film) : 
        camera(_camera),
        film(std::move(_film))
{
}

void Integrator::save(const std::string &path)
{
    film->save(path);
}
