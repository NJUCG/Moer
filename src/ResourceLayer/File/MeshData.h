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

#include "Eigen/Dense"

#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/Geometry/BoundingBox.h"

#include <string>
#include <vector>

class MeshData
{
public:
	friend class MeshDataManager;

	Eigen::MatrixXd m_vertices;
	
	Eigen::MatrixXd m_normals;

	Eigen::MatrixXd m_tangents;

	Eigen::MatrixXd m_bitangents;
	
	std::vector<Point2d> m_UVs;

	std::vector<Point3i> m_indices;

};