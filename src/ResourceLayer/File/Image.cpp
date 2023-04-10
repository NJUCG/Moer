#include "Image.h"
#include "ResourceLayer/File/FileUtils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

Image::Image() {
    imageRawData = nullptr;
}

Image::~Image() {
    delete[] imageRawData;
}

template<typename T>
T *Image::as() const {
    return reinterpret_cast<T *>(imageRawData);
}

Image::Image(const std::string &path, ImageLoadMode ilm) {

    // todo: support alpha reading
    // todo: do gamma correction
    isHdr = stbi_is_hdr(path.c_str());
    int w, h, c;
    if (isHdr) {
        float *data = stbi_loadf(path.c_str(), &w, &h, &c, 3);
        imageRawData = new float[w * h * c];
        std::memcpy(imageRawData, data, w * h * c * sizeof(float));
        free(data);
    } else
    // todo: bw mode
    {
        unsigned char *data = stbi_load(path.c_str(), &w, &h, &c, 3);
        if (!data) {
            std::cout << "Fail to load image " << path << std::endl;
            return;
        }
        imageRawData = new float[w * h * c];
        for (int k = 0; k < c; ++k) {
            for (int j = 0; j < h; ++j) {
                for (int i = 0; i < w; ++i) {
                    int destIdx = i + w * j + w * h * k;
                    int srcIdx = i + w * j + w * h * k;
                    // todo: gamma
                    as<float>()[destIdx] = (float)data[srcIdx] / 255;
                }
            }
        }
        free(data);
    }

    resolution = Point2i(w, h);
    channels = c;
}

Image::Image(const Point2i &resolution, int channels) : resolution(resolution), channels(channels) {
    int w = resolution.x;
    int h = resolution.y;
    int c = channels;
    imageRawData = new float[w * h * c];
}

Image::Image(const Point3i &shape) : resolution(shape.x, shape.y), channels(shape.z) {
    int w = resolution.x;
    int h = resolution.y;
    int c = channels;
    imageRawData = new float[w * h * c]();
}

Point2i Image::getResolution() const {
    return resolution;
}

int Image::getChannels() const {
    return channels;
}

int Image::getWidth() const {
    return resolution.x;
}

int Image::getHeight() const {
    return resolution.y;
}

void Image::setColorAt(const Point2i &p, const RGB3 &rgb) {
    int i = p.x;
    int j = p.y;
    int c = 3;
    int w = resolution.x;
    int srcIdx = c * i + c * w * j;

    as<float>()[srcIdx + 0] = rgb[0];
    as<float>()[srcIdx + 1] = rgb[1];
    as<float>()[srcIdx + 2] = rgb[2];
}

RGB3 Image::getRGBColorAt(const Point2i &p) {
    RGB3 ans;
    int i = p.x;
    int j = p.y;
    int c = 3;
    int w = resolution.x;
    int srcIdx = c * i + c * w * j;
    ans[0] = as<float>()[srcIdx + 0];
    ans[1] = as<float>()[srcIdx + 1];
    ans[2] = as<float>()[srcIdx + 2];
    return ans;
}

Spectrum Image::getSpectrumColorAt(const Point2i &p) {
    return Spectrum(getRGBColorAt(p));
}

bool Image::saveTo(const std::string &path) {
    auto destpath = FileUtils::getFilePath(path, "hdr", false);
    const char *destHdrPath = destpath.c_str();
    stbi_write_hdr(destHdrPath, resolution.x, resolution.y, 3, imageRawData);
    return true;
}
