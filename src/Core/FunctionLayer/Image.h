/**
 * @file Image.h
 * @author orbitchen
 * @brief Simple Image representation.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../CoreLayer/Geometry.h"
#include "../CoreLayer/Color.h"

#include <string>

class Image 
{
	char* imageRawData;

	Image();

	enum class ImageLoadMode { IMAGE_LOAD_BW, IMAGE_LOAD_COLOR };
	Image(std::string path, ImageLoadMode = ImageLoadMode::IMAGE_LOAD_COLOR);

public:

	// @brief generate one black image with resolution [width,height] and channels.
	Image(Point2i resolution, int channels);

	friend class ImageAllocator;
	
	Point2i getResolution() const;
	int getChannels() const;
	int getWidth() const;
	int getHeight() const;

	void setColorAt(Point2i p, const Spectrum& s);
	void setColorAt(Point2i p, const RGB3& rgb);

	bool saveTo(std::string path);
};