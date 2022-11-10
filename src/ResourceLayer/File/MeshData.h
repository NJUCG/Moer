/**
 * @file MeshData.h
 * @author orbitchen
 * @brief Mesh data for both real time renderer and ray tracing renderer.
 * @version 0.1
 * @date 2022-04-30
 * -	edit by Chenxi Zhou, 2022-8-14
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <string>
#include <vector>
#include "Eigen/Dense"
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Geometry/BoundingBox.h"

/// @brief Raw mesh data.
class MeshData
{
public:
	friend class MeshDataManager;

	/// @brief matrix for all vertices; vertices organized as column vectors.
	Eigen::MatrixXd m_vertices;
	
	/// @brief matrix for all normals; normals organized as column vectors.
	Eigen::MatrixXd m_normals;

	/// @brief matrix for all tangent vectors; tangent vectors organized as column vectors.
	Eigen::MatrixXd m_tangents;

	/// @brief matrix for all bitangent vectors; bitangent vectors organized as column vectors.
	Eigen::MatrixXd m_bitangents;
	
	/// @brief array for all uv coordinates.
	std::vector<Point2d> m_UVs;

	/// @brief array for all vectex indices.
	std::vector<Point3i> m_indices;

};