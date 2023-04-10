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

///@brief All type will be transformed to float
class ImageManager;

class Image {

    friend class ImageManager;

public:
    enum class ImageLoadMode {
        IMAGE_LOAD_BW,
        IMAGE_LOAD_COLOR
    };

private:
    float *imageRawData;

    Point2i resolution;

    int channels;

    bool isHdr = false;

    /// @brief load an image from path. Can ONLY be accessed from ImageManager. If you want to load an image from path, call ImageManager::getInstance::getImage.
    /// @param path path for the image.
    /// @param ilm load mode for image, color or black/white.
    Image(const std::string &path, ImageLoadMode ilm = ImageLoadMode::IMAGE_LOAD_COLOR);

public:
    template<typename T>
    inline T *as() const;

    Image();
    ~Image();

    // @brief generate one black image with resolution [width,height] and channels. Could be accessed directly.
    Image(const Point2i &resolution, int channels);
    Image(const Point3i &shape);

    Point2i getResolution() const;
    int getChannels() const;
    int getWidth() const;
    int getHeight() const;

    void setColorAt(const Point2i &p, const Spectrum &s) {
        setColorAt(p, s.toRGB3());
    }

    void setColorAt(const Point2i &p, const RGB3 &rgb);
    RGB3 getRGBColorAt(const Point2i &p);
    Spectrum getSpectrumColorAt(const Point2i &p);

    bool saveTo(const std::string &path);
};
