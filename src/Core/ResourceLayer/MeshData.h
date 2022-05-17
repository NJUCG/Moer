/**
 * @file MeshData.h
 * @author orbitchen
 * @brief Mesh data for both real time renderer and ray tracing renderer.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../CoreLayer/Geometry.h"

#include <string>

class MeshData
{
	double *vertexRaw;
	double *normalRaw;
	double *uvRaw;
	double *tangentRaw;
	double *bitangentRaw;
	int *indiceRaw;

	// @brief init MeshData from raw data pointer. MeshData can not be initialized from file path cause one single file may cantain multiple MeshData.
	MeshData(double *_v, double *_n, double *_uv, double *_tan, double *_bi, int *_indice);

public:
	friend class MeshDataManager;

	Point3d getVertexAt(int i) const;
	Normal3d getNormalAt(int i) const;
	Point2d getUvAt(int i) const;
	Vec3d getTangentAt(int i) const;
	Vec3d getBitangentAt(int i) const;

	// @brief get 3 indices for ist triangle mesh. In order.
	Point3i getTriangleIndiceAt(int i) const;

	int getTriangleNum() const;
};