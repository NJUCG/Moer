/**
 * @file bvh.h
 * @author Pengpei Hong
 * @brief BVH implementation.
 * @version 0.1
 * @date 2022-06-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#include "Bvh.h"
#include "FunctionLayer/Shape/Mesh.h"

#define BVH_BUILD_ERROR false
//build the BVH of entites within the interval [start, end)
std::shared_ptr<BvhTreeNode> Bvh::RecursiveBuild(std::vector<EntityInfo>& entityInfo, 
												 int start, int end, int& nodeNumber, 
												 std::vector<int>& orderedIndices) {
	nodeNumber++;
	std::shared_ptr<BvhTreeNode> root = std::make_shared<BvhTreeNode>();
	BoundingBox3f totalBounds;
	for (int i = start; i < end; i++) totalBounds = BoundingBoxUnion(totalBounds, entityInfo[i].bounds);
	root->bounds = totalBounds;
	int nEntities = end - start;
	if (nEntities == 1) {
		//leaf node
		root->entityOffset = orderedIndices.size();
		root->nEntites = 1;
		orderedIndices.push_back(entityInfo[start].EntityId);
	}
	else {
		BoundingBox3f centerBounds;
		for (int i = start; i < end; i++) centerBounds = BoundingBoxUnion(centerBounds, BoundingBox3f(entityInfo[i].center));
		int dim = -1;
		double maxD = 0;
		for (int i = 0; i < 3; i++) {
			double D = centerBounds.pMax[i] - centerBounds.pMin[i];
			if (maxD < D) maxD = D, dim = i;
		}
		if (dim == -1) {
			//leaf node
			root->entityOffset = orderedIndices.size();
			root->nEntites = nEntities;
			for (int i = start; i < end; i++) {
				orderedIndices.push_back(entityInfo[i].EntityId);
			}
		}
		else {
			//interior node
			root->splitAxis = dim;
			int mid = (start + end) >> 1, success = 1;

			if (splitMethod == SplitMethod::Middle) {
				double pmid = 0.5 * (centerBounds.pMin[dim] + centerBounds.pMax[dim]);
				mid = std::partition(entityInfo.begin() + start, entityInfo.begin() + end, [&](const EntityInfo& pi) {
					return pi.center[dim] < pmid;
				}) - entityInfo.begin();
				if (mid == start || mid == end) success = 0;
			}

			else if (nEntities <= 2 || splitMethod == SplitMethod::EqualCounts) {
				mid = (start + end) >> 1;
				std::nth_element(entityInfo.begin() + start, 
								 entityInfo.begin() + mid, 
								 entityInfo.begin() + end, 
								 [&](const EntityInfo& a, const EntityInfo& b) {
					return a.center[dim] < b.center[dim];
				});
				success = 1;
			}

			else if (!success || splitMethod == SplitMethod::SAH) {
				const int split = 30;
				std::vector<std::pair<int, BoundingBox3f>> bucketInfos(split);
				std::vector<BoundingBox3f> suffixBounds(split);
				for (int i = start; i < end; i++) {
					int bucket = split * (entityInfo[i].center[dim] - centerBounds.pMin[dim]) / maxD;
					if (bucket == split) bucket--;
					auto& [count, bounds] = bucketInfos[bucket];
					count++;
					bounds = BoundingBoxUnion(bounds, entityInfo[i].bounds);
				}
				for (int i = split - 1; i >= 0; i--) {
					suffixBounds[i] = bucketInfos[i].second;
					if (i + 1 < split) suffixBounds[i] = BoundingBoxUnion(suffixBounds[i], suffixBounds[i + 1]);
				}
				double minCost = DBL_MAX;
				int count0 = 0, minCostBucket = -1;
				BoundingBox3f prefix;
				for (int i = 0; i + 1 < split; i++) {
					const auto [count, bounds] = bucketInfos[i];
					count0 += count;
					prefix = BoundingBoxUnion(prefix, bounds);
					double costI = 1 + (count0 * prefix.SurfaceArea() + (nEntities - count0) * suffixBounds[i + 1].SurfaceArea()) / totalBounds.SurfaceArea();
					if (minCost > costI) {
						minCost = costI;
						minCostBucket = i;
					}
				}
				if (minCost < nEntities) {
					mid = std::partition(entityInfo.begin() + start, entityInfo.begin() + end, [&](const EntityInfo& pi) {
						int bucket = split * (pi.center[dim] - centerBounds.pMin[dim]) / maxD;
						if (bucket == split) bucket--;
						return bucket <= minCostBucket;
					}) - entityInfo.begin();
				}
				else {
					//leaf node
					mid = -1;
					root->entityOffset = orderedIndices.size();
					root->nEntites = nEntities;
					for (int i = start; i < end; i++) {
						orderedIndices.push_back(entityInfo[i].EntityId);
					}
				}
			}
			else assert(BVH_BUILD_ERROR);//TODO:should not be here!!!
			if (mid != -1) {
				assert(mid > start);
				root->children[0] = RecursiveBuild(entityInfo, start, mid, nodeNumber, orderedIndices);
				root->children[1] = RecursiveBuild(entityInfo, mid, end, nodeNumber, orderedIndices);
			}
		}
	}
	return root;
}

void Bvh::Flatten(std::shared_ptr<BvhTreeNode> node, int& dfsOrder) {
	if (node == nullptr) return;
	auto& lnode = linearBvhNodes[dfsOrder++];
	lnode.bounds = node->bounds;
	if (node->nEntites > 0) {
		//leaf
		lnode.firstdEntityOffset = node->entityOffset;
		lnode.nEntites = node->nEntites;
	}
	else {
		//interior
		lnode.splitAxis = node->splitAxis;
		Flatten(node->children[0], dfsOrder);
		lnode.secondChildOrder = dfsOrder;
		Flatten(node->children[1], dfsOrder);
	}
}

Bvh::Bvh(std::vector<std::shared_ptr<Entity>>& _entites, SplitMethod _splitMethod): entites(_entites), splitMethod(_splitMethod){
	if (entites.empty()) return;

	std::vector<EntityInfo> entityInfo(entites.size());
	for (int i = 0; i < entites.size(); i++) entityInfo[i] = EntityInfo(i, entites[i]->WorldBound());
	std::vector<int> orderedIndices;
	int nodeNumber = 0;
	std::shared_ptr<BvhTreeNode> root = RecursiveBuild(entityInfo, 0, entityInfo.size(), nodeNumber, orderedIndices);

	indices.swap(orderedIndices);
	entityInfo.resize(0);
	
	int dfsOrder = 0;
	linearBvhNodes.resize(nodeNumber);
	Flatten(root, dfsOrder);
}

Bvh::Bvh(Mesh *_triangleMesh, 
		 SplitMethod _splidMethod) :
	triangleMesh(_triangleMesh), splitMethod(_splidMethod)
{
	const auto &idxes = triangleMesh->m_indices;
	const auto &vertices = triangleMesh->m_vertices;
	if (idxes.empty()) return;

	auto getTriangleBound = [&idxes, &vertices](int i) {
		auto [i0, i1, i2] = idxes[i];
		auto p0 = eigenToPoint3d(vertices.col(i0)),
			 p1 = eigenToPoint3d(vertices.col(i1)),
			 p2 = eigenToPoint3d(vertices.col(i2));
		BoundingBox3f bounds{p0};
		bounds = BoundingBoxUnion(bounds, BoundingBox3f{p1});
		bounds = BoundingBoxUnion(bounds, BoundingBox3f{p2});
		return bounds;
	};

	std::vector<EntityInfo> entityInfo(idxes.size());
	for (int i = 0; i < idxes.size(); ++i) {
		entityInfo[i] =  EntityInfo{i, getTriangleBound(i)};
	}

	std::vector<int> orderedIndices;
	int nodeNumber = 0;
	std::shared_ptr<BvhTreeNode> root = RecursiveBuild(entityInfo, 0, entityInfo.size(), nodeNumber, orderedIndices);

	indices.swap(orderedIndices);
	entityInfo.resize(0);
	
	int dfsOrder = 0;
	linearBvhNodes.resize(nodeNumber);
	Flatten(root, dfsOrder);
}

std::optional<Intersection> Bvh::Intersect(const Ray& r) const{
	if (linearBvhNodes.empty()) return std::nullopt;
	Ray R(r);
	Vec3d invDir(1 / r.direction[0], 1 / r.direction[1], 1 / r.direction[2]);
	bool isNegDir[3] = { r.direction[0] < 0, r.direction[1] < 0, r.direction[2] < 0 };
	std::vector<int> nextNodeStack;
	int currentNode = 0, flag = 0;
	std::optional<Intersection> result;
	while (true) {
		auto& node = linearBvhNodes[currentNode];
		auto boundingIts = node.bounds.Intersection(R);
		if (boundingIts.has_value()) {
			if (node.nEntites > 0) {
				for (int i = 0; i < node.nEntites; i++) {
					int idx = indices[node.firstdEntityOffset +i];
					//auto its = entites[node.firstdEntityOffset + i]->intersect(R);
					std::optional<Intersection> its = std::nullopt;
					if (!entites.empty())
						its = entites[idx]->intersect(R);
					else {
						auto tri = triangleMesh->getTriangle(idx);
						its = tri.intersect(R);
						if (its.has_value())
							its->object = triangleMesh;
					}
					
					if (its.has_value()) {
						flag = true;
						double t = (its->position[0] - r.origin[0]) / r.direction[0];
						if (R.timeMax > t) {
							R.timeMax = t;
							result = its;
							result->t = t;
						}
					}
				}
				if (nextNodeStack.empty()) break;
				currentNode = nextNodeStack.back();
				nextNodeStack.pop_back();
			}
			else {
				if (isNegDir[node.splitAxis]) {
					nextNodeStack.push_back(currentNode + 1);
					currentNode = node.secondChildOrder;
				}
				else {
					nextNodeStack.push_back(node.secondChildOrder);
					currentNode = currentNode + 1;
				}
			}
		}
		else {
			if (nextNodeStack.empty()) break;
			currentNode = nextNodeStack.back();
			nextNodeStack.pop_back();
		}
	}
	return flag? result: std::nullopt;
}
