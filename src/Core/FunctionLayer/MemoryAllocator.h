/**
 * @file MemoryAllocator.h
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
class MemoryAllocator 
{
protected:

	std::map<std::string, std::shared_ptr<BaseType>> hash;

public:

	MemoryAllocator();

};

class ImageAllocator :
	public MemoryAllocator<Image>
{
	ImageAllocator();
	std::shared_ptr<ImageAllocator> instance;

public:
	// @brief singleton pattern get.
	static std::shared_ptr<ImageAllocator> get();

	std::shared_ptr<Image> getImage(std::string path, Image::ImageLoadMode mode);
};

class MeshDataAllocator :
	public MemoryAllocator<MeshData>
{
	MeshDataAllocator();
	std::shared_ptr<MeshDataAllocator> instance;

public:

	// @brief singleton pattern get.
	static std::shared_ptr<MeshDataAllocator> get();

	std::vector<std::shared_ptr<MeshData>> getMeshData(std::string path);



};