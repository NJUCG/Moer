/**
 * @file Film.cpp
 * @author Zhimin Fan
 * @brief The class that records ray sampling results. Just like old camera exposures!
 * @version 0.1
 * @date 2022-05-31
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../Core/FunctionLayer/Film.h"
#include <fstream>

Film::Film(Point2i resolution, int channels) : resolution(resolution), channels(channels)
{
    image = std::make_unique<Image>(resolution, channels);
    sumWeights.resize(resolution.x * resolution.y);
}

Spectrum Film::getSpectrum(const Point2i &p)
{
    return 0.0;
}

RGB3 Film::getRGB(const Point2i &p)
{
    return 0.0;
}

void Film::deposit(const Point2d &p, const Spectrum &s)
{
    // ignore filter now
    Point2i pi(p.x, p.y);
    int id = pi.y * resolution.x + pi.x;
    double originalW = sumWeights[id];
    sumWeights[id] += 1.0;
    double currentW = sumWeights[id];
    auto original = image->getSpectrumColorAt(pi);
    auto current = (originalW * original + s) / currentW;
    image->setColorAt(pi, s);

    // debug
    static std::ofstream ofs("render_result.txt");
    ofs << p.x << " " << p.y << " " << s.average() << std::endl;
}

void Film::save(const std::string &path)
{
    image->saveTo(path);
}

Point2i Film::getResolution() const
{
    return resolution;
}
