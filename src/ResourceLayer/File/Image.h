/**
 * @file Image.h
 * @author orbitchen
 * @brief Simple Image representation.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <string>
#include "CoreLayer/Geometry/Geometry.h"
#include "CoreLayer/ColorSpace/Color.h"

// todo: support various data type

class Image
{
	void  * imageRawData;
	Point2i resolution;
	int channels;

    bool isHdr = false;
public:

    template<typename  T>
    inline  T * as() const;

	Image();
	~Image();

	enum class ImageLoadMode
	{
		IMAGE_LOAD_BW,
		IMAGE_LOAD_COLOR
	};
	// todo: support alpha reading
	// todo: do gamma correction
	Image(const std::string &path, ImageLoadMode = ImageLoadMode::IMAGE_LOAD_COLOR);

	// @brief generate one black image with resolution [width,height] and channels.
	Image(const Point2i &resolution, int channels);
	Image(const Point3i &shape);

	friend class ImageManager;

	Point2i getResolution() const;
	int getChannels() const;
	int getWidth() const;
	int getHeight() const;

	void setColorAt(const Point2i &p, const Spectrum &s){
        setColorAt(p, s.toRGB3());
    }

	void setColorAt(const Point2i &p, const RGB3 &rgb);
	RGB3 getRGBColorAt(const Point2i &p);
	Spectrum getSpectrumColorAt(const Point2i &p);

	bool saveTo(const std::string &path);
};

