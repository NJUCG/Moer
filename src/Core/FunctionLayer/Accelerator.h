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

#include "AccelerateNode.h"
#include "Intersection.h"

#include <memory>
#include <vector>
#include "Entity.h"

class Accelerator 	
{
public:
	// ! I don't think intersect should be const
	// * Some datastructure may adjust while accessing
	virtual Intersection intersect(const Ray& r) const = 0;
	virtual void build(const std::vector<std::shared_ptr<Entity>>& entities) = 0;
};

class Bvh: public Accelerator
{
	std::shared_ptr<BvhNode> root;

protected:
	// // @return true if node is successfully added, false otherwise.
	// bool addAccelerateNode(std::shared_ptr<BvhNode> node);

	// // @return true if Bvh have this node, false otherwise.
	// bool have(std::shared_ptr<BvhNode> node) const;

	// @brief Ray intersect with a bounding box (NOT an object or Entity). return intersected BvhNode or nullptr.
	std::shared_ptr<BvhNode> intersect(const Ray& r);

public:
	Bvh();

	virtual Intersection intersect(const Ray& r) const;
	virtual void build(const std::vector<std::shared_ptr<Entity>>& entities);
};