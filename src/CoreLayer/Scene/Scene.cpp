/**
 * @file Scene.cpp
 * @author Zhimin Fan
 * @brief Scene implemention.
 * @version 0.1
 * @date 2022-05-31
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "Scene.h"

Scene::Scene() : lights(std::make_shared<std::vector<std::shared_ptr<Light>>>()), entities(std::make_shared<std::vector<std::shared_ptr<Entity>>>())
{
}

void Scene::build() {
	BVH = std::make_shared<Bvh>(*entities);
}
std::optional<Intersection> Scene::intersect(const Ray &r) const
{
    if (BVH)
        return BVH->Intersect(r);
    std::optional<Intersection> minIntersection;
    for (auto i : *entities)
    {
        auto its = i->intersect(r);
        if (its.has_value())
        {
            if (minIntersection.has_value())
            {
                double d = (its->position - r.origin).length2();
                double d0 = (minIntersection->position - r.origin).length2();
                if (d < d0)
                {
                    minIntersection = its;
                }
            }
            else
            {
                minIntersection = its;
            }
        }
    }
    return minIntersection;
}

std::shared_ptr<std::vector<std::shared_ptr<Light>>> Scene::getLights() const
{
    return lights;
}

void Scene::addEntity(std::shared_ptr<Entity> object)
{
    entities->push_back(object);
}

void Scene::addLight(std::shared_ptr<Light> light)
{
    lights->push_back(light);
}
