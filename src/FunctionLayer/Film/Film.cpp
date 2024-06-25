/**
 * @file Film.cpp
 * @author Zhimin Fan
 * @brief The class that records ray sampling results. Just like old camera exposures!
 * @version 0.1
 * @date 2022-05-31
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include <fstream>
#include "Film.h"

Film::Film(Point2i resolution, int channels) : resolution(resolution), channels(channels) {
    image = std::make_unique<Image>(resolution, channels);
    sumWeights.resize(resolution.x * resolution.y);
    sumValues.resize(resolution.x * resolution.y);
}

Spectrum Film::getSpectrum(const Point2i &p) {
    return 0.0;
}

RGB3 Film::getRGB(const Point2i &p) {
    return 0.0;
}

void Film::deposit(const Point2i &p, const Spectrum &s) {
    // ! A temp implementation
    // ignore filter now
    if(s.hasNaN() || s.luminance()<0){
        std::runtime_error("Invalid spectrum");
    }
    int id = p.y * resolution.x + p.x;
    sumWeights[id] += 1.0;
    sumValues[id] += s;
}

void Film::save(const std::string &path) {
    // ! A temp implementation
#ifdef _DEBUG
    std::ofstream ofs("render_result.txt");
#endif
    for (int i = 0; i < resolution.y; i++) {
        for (int j = 0; j < resolution.x; j++) {
            int id = i * resolution.x + j;
            Spectrum value = sumValues[id] / sumWeights[id];
            image->setColorAt(Point2i(j, i), value);
#ifdef _DEBUG
             ofs << j << " " << i << " " << value[0] << " " << value[1] << " " << value[2] << " " << std::endl;
#endif
        }
    }
    image->saveTo(path);
}

Point2i Film::getResolution() const {
    return resolution;
}

Spectrum Film::postProcess(const Spectrum &value) const {
    RGB3 rgbValue = ToneMapping::toneMap(toneMapType, value.toRGB3());
    return Spectrum(rgbValue);
}
