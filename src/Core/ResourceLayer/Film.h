/**
 * @file Film.h
 * @author orbitchen
 * @brief The class that records ray sampling results. Just like old camera exposures!
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../FunctionLayer/MemoryAllocator.h"

#include <memory>

class Film
{
protected:

	std::shared_ptr<Image> image;

	// TODO save accumulated spectrum. HOW?

	void syncWithGui();

public:

	Film(Point2i resolution, int channels);
	Film(Point3i shape);

	Spectrum getSpectrum(const Point2i& p);
	RGB3 getRGB(const Point2i& p);

	void setGamma(double gamma=2.2);

	// @brief: 'deposit' a spectrum at p. all deposit will be averaged when get(p).
	void deposit(const Point2i& p, const Spectrum& s);

	int getDepositeCount(const Point2i& p);
};