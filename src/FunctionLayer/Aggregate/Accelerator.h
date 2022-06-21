/**
 * @file Accelerator.h
 * @author orbitchen
 * @brief Accelerator. Be inherited means that the inheriting class supports accelerate seraching algorithm.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <vector>
#include <memory>
#include "AccelerateNode.h"
#include "FunctionLayer/Intersection.h"
#include "FunctionLayer/Shape/Entity.h"

class Accelerator
{
protected:

	// @brief Ray intersect with a bounding box (NOT an object or Entity). return intersected BvhNode or nullptr.
	virtual std::shared_ptr<BvhNode> boundingIntersect(const Ray& r) = 0;

public:

	virtual Intersection intersect(const Ray &r) = 0;
	virtual void build(const std::vector<std::shared_ptr<Entity>> &entities) = 0;
};

class Bvh : public Accelerator
{
	std::unique_ptr<BvhNode> root;

protected:

	virtual std::shared_ptr<BvhNode> boundingIntersect(const Ray& r);

public:
	Bvh();

	virtual Intersection intersect(const Ray &r) const;
	virtual void build(const std::vector<std::shared_ptr<Entity>> &entities);
};