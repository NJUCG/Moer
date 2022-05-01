/**
 * @file AccelerateNode.h
 * @author orbitchen
 * @brief Accelerate node. Be inherited means that the inheriting class can be as a node for acceleration structure.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../CoreLayer/Color.h"
#include "../CoreLayer/BoundingBox.h"

#include <memory>
#include <vector>
#include "Entity.h"

class BvhNode
{
protected:

	std::shared_ptr<BvhNode> leftNode;
	std::shared_ptr<BvhNode> rightNode;

	std::vector<std::shared_ptr<Entity>> entities;

public:

	BoundingBox3f getBoundingBox() const;

};