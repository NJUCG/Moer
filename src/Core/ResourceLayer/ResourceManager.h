/**
 * @file ResourceManager.h
 * @author orbitchen
 * @brief Simple Memory Allocator.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Image.h"
#include "MeshData.h"

#include <map>
#include <string>
#include <memory>

template <typename BaseType>
class ResourceManager
{
protected:
	std::map<std::string, std::shared_ptr<BaseType>> hash;

public:
	ResourceManager();
};

class ImageManager : public ResourceManager<Image>
{
	ImageManager();
	std::shared_ptr<ImageManager> instance;

public:
	// @brief singleton pattern get.
	static std::shared_ptr<ImageManager> getInstance();

	std::shared_ptr<Image> getImage(const std::string &path, Image::ImageLoadMode mode);
};

class MeshDataManager : public ResourceManager<MeshData>
{
	MeshDataManager();
	std::shared_ptr<MeshDataManager> instance;

public:
	// @brief singleton pattern get.
	static std::shared_ptr<MeshDataManager> getInstance();

	std::vector<std::shared_ptr<MeshData>> getMeshData(const std::string &path);
};