#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

Image::Image()
{
    imageRawData = nullptr;
}

Image::~Image()
{
    delete[] imageRawData;
}

Image::Image(const std::string &path, ImageLoadMode ilm)
{
    // todo: bw mode
    int w, h, c;
    unsigned char *data = stbi_load(path.c_str(), &w, &h, &c, 3);
    if (!data)
    {
        std::cout << "Fail to load image " << path << std::endl;
        return;
    }
    imageRawData = new unsigned char[w * h * c];
    for (int k = 0; k < c; ++k)
    {
        for (int j = 0; j < h; ++j)
        {
            for (int i = 0; i < w; ++i)
            {
                int destIdx = i + w * j + w * h * k;
                int srcIdx = i + w * j + w * h * k;
                // todo: gamma
                imageRawData[destIdx] = data[srcIdx];
            }
        }
    }
    free(data);
    resolution = Point2i(w, h);
    channels = c;
}

Image::Image(const Point2i &resolution, int channels) : resolution(resolution), channels(channels)
{
    int w = resolution.x;
    int h = resolution.y;
    int c = channels;
    imageRawData = new unsigned char[w * h * c];
}

Image::Image(const Point3i &shape) : resolution(shape.x, shape.y), channels(shape.z)
{
    int w = resolution.x;
    int h = resolution.y;
    int c = channels;
    imageRawData = new unsigned char[w * h * c];
    memset(imageRawData, 0, w * h * c * sizeof(unsigned char));
}

Point2i Image::getResolution() const
{
    return resolution;
}

int Image::getChannels() const
{
    return channels;
}

int Image::getWidth() const
{
    return resolution.x;
}

int Image::getHeight() const
{
    return resolution.y;
}

void Image::setColorAt(const Point2i &p, const Spectrum &s)
{
    setColorAt(p, s.toRGB3());
}

void Image::setColorAt(const Point2i &p, const RGB3 &rgb)
{
    int i = p.x;
    int j = p.y;
    int c = 3;
    int w = resolution.x;
    int srcIdx = c * i + c * w * j;

    int r = std::min(1.0, std::max(0.0, rgb[0])) * 255;
    int g = std::min(1.0, std::max(0.0, rgb[1])) * 255;
    int b = std::min(1.0, std::max(0.0, rgb[2])) * 255;

    imageRawData[srcIdx + 0] = r;
    imageRawData[srcIdx + 1] = g;
    imageRawData[srcIdx + 2] = b;
}

RGB3 Image::getRGBColorAt(const Point2i &p)
{
    RGB3 ans;
    int i = p.x;
    int j = p.y;
    int c = 3;
    int w = resolution.x;
    int srcIdx = c * i + c * w * j;
    ans[0] = imageRawData[srcIdx + 0] * 1. / 255;
    ans[1] = imageRawData[srcIdx + 1] * 1. / 255;
    ans[2] = imageRawData[srcIdx + 2] * 1. / 255;
    return ans;
}

Spectrum Image::getSpectrumColorAt(const Point2i &p)
{
    return Spectrum(getRGBColorAt(p));
}

bool Image::saveTo(const std::string &path)
{
    stbi_write_bmp(path.c_str(), resolution.x, resolution.y, 3, imageRawData);
    return true;
}
