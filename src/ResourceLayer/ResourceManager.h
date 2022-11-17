/**
 * @file ResourceManager.h
 * @author orbitchen
 * @brief Simple Memory Allocator.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <memory>
#include "ResourceLayer/File/Image.h"
#include "ResourceLayer/File/MeshData.h"

/// @brief Manager for 'heavy' resources.
/// @tparam BaseType type of resources, could be mesh or image.
template <typename BaseType>
class ResourceManager
{
protected:

	/// @brief hash for saving resources.
	/// key: full file path for resource.
	/// value: shared ptr for resource.
	std::map<std::string, std::shared_ptr<BaseType>> hash;

public:
	ResourceManager() { };
};

class ImageManager : public ResourceManager<Image>
{
	static std::shared_ptr<ImageManager> instance;

public:
	// @brief singleton pattern get.
	static std::shared_ptr<ImageManager> getInstance();

	std::shared_ptr<Image> getImage(const std::string &path, Image::ImageLoadMode mode=Image::ImageLoadMode::IMAGE_LOAD_COLOR);
};

/// @brief mesh data collection for simplifying.
using MeshDataCollection=std::unordered_map<std::string,std::shared_ptr<MeshData>>;

class MeshDataManager : public ResourceManager<MeshDataCollection>
{
	static std::shared_ptr<MeshDataManager> instance;

public:
	// @brief singleton pattern get.
	static std::shared_ptr<MeshDataManager> getInstance();

    std::shared_ptr<MeshDataCollection> getMeshData(const std::string &path);
};
