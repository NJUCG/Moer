/**
 * @file bvh.h
 * @author Pengpei Hong
 * @brief BVH declaration.
 * @version 0.1
 * @date 2022-06-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "Accel.h"
#include "CoreLayer/Geometry/BoundingBox.h"
#include "FunctionLayer/Shape/Entity.h"
#include "FunctionLayer/Intersection.h"

/// @brief Entity information declaration for building BVH
struct EntityInfo {
	EntityInfo(){}
	EntityInfo(int _EntityId, const BoundingBox3f& _bounds): EntityId(_EntityId), bounds(_bounds), center(0.5 * (_bounds.pMin + _bounds.pMax)){}
	int EntityId;
	BoundingBox3f bounds;
	Point3d center;
};

struct BvhTreeNode {
	BoundingBox3f bounds;
	std::shared_ptr<BvhTreeNode> children[2] = {nullptr, nullptr};//0: left, 1: right
	int splitAxis;
	int nEntites = 0;//0: interior nodes, otherwise: leaf nodes
	int entityOffset;
};

/// @brief Bvh Nodes in Dfs-Order
struct LinearBvhNode {
	BoundingBox3f bounds;
	union
	{
		int firstdEntityOffset;//for leaves to enumerate
		int secondChildOrder;//for interior nodes to traverse
	};
	int nEntites = 0;
	int splitAxis;
};

class Mesh;

/// @brief Bounding Volume Hierarchies.
struct Bvh : public Accel{
public:
	const enum class SplitMethod{
		SAH, 
		Middle, 
		EqualCounts
	} splitMethod;//not support LBVH yet

	std::vector<LinearBvhNode> linearBvhNodes;
	
	std::vector<int> indices;
	
	//* If entities bvh
	std::vector<std::shared_ptr<Entity>> entites;

	//* If triangles bvh
	Mesh *triangleMesh;


	/**
	 * @brief Bvh constructor
	 * @param <_entites>
	 * @param <_SplitMeshod>
	 */
	Bvh(std::vector<std::shared_ptr<Entity>>& _entites, SplitMethod _splitMethod = SplitMethod::SAH);

	Bvh(Mesh *_triangleMesh, SplitMethod _splidMethod = SplitMethod::SAH);

	/**
	* @brief recursively build BVH
	* @return the root of BVH
	*/
	std::shared_ptr<BvhTreeNode> RecursiveBuild(std::vector<EntityInfo>& entityInfo, 
												int start, int end, int& nodeNumber, 
												std::vector<int>& orderedIndices);

	/// @brief flatten the BVH to Dfs-Order
	void Flatten(std::shared_ptr<BvhTreeNode> node, int& dfsOrder);

	/// @brief return the scene intersection
	virtual std::optional<Intersection> Intersect(const Ray& r) const override;

    [[nodiscard]]
    BoundingBox3f getGlobalBoundingBox() const override {
        return linearBvhNodes[0].bounds;
    }
};
