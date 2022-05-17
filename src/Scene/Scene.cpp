/**
 * @file Scene.cpp
 * @author Zhimin Fan
 * @brief Ray implemention.
 * @version 0.1
 * @date 2022-05-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../Core/FunctionLayer/Scene.h"

Intersection Scene::intersect(const Ray &r) const
{
    // todo
    return Intersection();
}

std::shared_ptr<std::vector<std::shared_ptr<Light>>> Scene::getLights() const
{
    // todo
    return nullptr;
}
