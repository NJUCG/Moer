/**
 * @file Film.h
 * @author orbitchen
 * @brief The class that records ray sampling results. Just like old camera exposures!
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <memory>
#include "ToneMapping.h"
#include "FunctionLayer/Filter/Filter.h"
#include "ResourceLayer/ResourceManager.h"

class Film
{
protected:
	std::unique_ptr<Image> image;
	std::shared_ptr<Filter> filter;
	Point2i resolution;
	int channels;
	std::vector<double> sumWeights; // a temp impl
	std::vector<Spectrum> sumValues; // a temp impl
    ToneMapping::ToneMapType toneMapType = ToneMapping::Filmic;
	void syncWithGui();

public:
	Film(Point2i resolution, int channels);
	Film(Point3i shape);

	Spectrum getSpectrum(const Point2i &p);
	RGB3 getRGB(const Point2i &p);

	void setGamma(double gamma = 2.2);
	void setFilter(std::shared_ptr<Filter> filter);
	Point2i getResolution() const;

	// @brief: 'deposit' a spectrum at p. all deposit will be averaged when get(p).
	void deposit(const Point2i &p, const Spectrum &s);
	void save(const std::string &path);

    Spectrum postProcess(const Spectrum & value) const;

	int getDepositeCount(const Point2i &p);
};