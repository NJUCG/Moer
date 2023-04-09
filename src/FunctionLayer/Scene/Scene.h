/**
 * @file Scene.h
 * @author orbitchen
 * @brief Scene representation. Handle ray intersection.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <optional>
#include "nlohmann/json.hpp"
#include "CoreLayer/Ray/Ray.h"
#include "FunctionLayer/Shape/Entity.h"
#include "FunctionLayer/Acceleration/Bvh.h"
#include "FunctionLayer/Acceleration/Accel.h"
#include "FunctionLayer/Intersection.h"
#include "FunctionLayer/Light/Light.h"
#include "FunctionLayer/Medium/Medium.h"

/// \brief Store the primitives in scene
class Scene
{
	std::shared_ptr<Accel> accel;											///< Spacial accelerate structure
																		///< \todo Replace it whit a abstruct base class
	std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights;
	std::shared_ptr<std::vector<std::shared_ptr<Entity>>> entities;
    std::unordered_map<std::string,std::shared_ptr<Material>> materials;
    std::unordered_map<std::string,std::shared_ptr<Medium>> mediums;

public:
	Scene();
    Scene(const Json & json);
	void addEntity(std::shared_ptr<Entity> object);
	void addLight(std::shared_ptr<Light> light);

	void build();
	std::optional<Intersection> intersect(const Ray &r) const;

	// @return true if r hits object first (closest), false otherwise.
	bool intersectionTest(const Ray &r, std::shared_ptr<Entity> object) const;

    [[nodiscard]]
    BoundingBox3f getGlobalBoundingBox() const {
        return accel->getGlobalBoundingBox();
    }

	std::shared_ptr<std::vector<std::shared_ptr<Light>>> getLights() const;

    std::shared_ptr<Material> fetchMaterial(const std::string & name = "default") const;
    std::shared_ptr<Medium>   fetchMedium(const std::string & name) const;

};