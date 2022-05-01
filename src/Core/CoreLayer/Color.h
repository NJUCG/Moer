/**
 * @file Color.h
 * @author orbitchen
 * @brief Color representation, including rgb, xyz and spectrum.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Geometry.h"

#include <vector>

class RGB3;
class XYZ3;
class Spectrum;

// @brief the number of uniform samples for SampledSpectrum.
static const int nSpectrumSamples = 60;

class RGB3
{
	double rgbData[3];

public:
	RGB3();

	RGB3(double r, double g, double b);

	// @brief initialize all rgb value as val.
	RGB3(double val);

	double operator[](int i);
	double &operator[](int i);

	RGB3 operator+(const RGB3 &rgb);
	RGB3 operator-(const RGB3 &rgb);
	RGB3 operator*(const RGB3 &rgb);
	RGB3 operator/(const RGB3 &rgb);

	RGB3 &operator+=(const RGB3 &rgb);
	RGB3 &operator-=(const RGB3 &rgb);
	RGB3 &operator*=(const RGB3 &rgb);
	RGB3 &operator/=(const RGB3 &rgb);

	RGB3 operator*(double v);
	RGB3 operator/(double v);

	RGB3 &operator*=(double v);
	RGB3 &operator/=(double v);

	friend RGB3 operator*(double v, const RGB3 &rgb);

	XYZ3 toXYZ3() const;

	// @brief convert RGB3 to SampledSpectrum.
	Spectrum toSpectrum() const;
};

class XYZ3
{
	double xyzData[3];

public:
	XYZ3();

	XYZ3(double x, double y, double z);

	// @brief initialize all xyz value as val.
	XYZ3(double val);

	double operator[](int i);
	double &operator[](int i);

	XYZ3 operator+(const XYZ3 &xyz);
	XYZ3 operator-(const XYZ3 &xyz);
	XYZ3 operator*(const XYZ3 &xyz);
	XYZ3 operator/(const XYZ3 &xyz);

	XYZ3 &operator+=(const XYZ3 &xyz);
	XYZ3 &operator-=(const XYZ3 &xyz);
	XYZ3 &operator*=(const XYZ3 &xyz);
	XYZ3 &operator/=(const XYZ3 &xyz);

	XYZ3 operator*(double v);
	XYZ3 operator/(double v);

	XYZ3 &operator*=(double v);
	XYZ3 &operator/=(double v);

	friend XYZ3 operator*(double v, const XYZ3 &xyz);

	RGB3 toRGB3() const;
};

// @brief Spectrum of nSamples sample points.
template <int nSamples>
class Spectrum
{
protected:
	double coefficients[nSamples];

public:
	// @brief all coefficients initialized as 0.0f.
	Spectrum();

	// @brief all coefficients initialized as val.
	Spectrum(double val);

	double operator[](int i);
	double &operator[](int i);

	Spectrum operator+(const Spectrum &s);
	Spectrum operator-(const Spectrum &s);
	Spectrum operator*(const Spectrum &s);
	Spectrum operator/(const Spectrum &s);

	Spectrum &operator+=(const Spectrum &s);
	Spectrum &operator-=(const Spectrum &s);
	Spectrum &operator*=(const Spectrum &s);
	Spectrum &operator/=(const Spectrum &s);

	Spectrum operator*(double v);
	Spectrum operator/(double v);

	Spectrum &operator*=(double v);
	Spectrum &operator/=(double v);

	friend Spectrum operator*(double v, const Spectrum &s);

	friend Spectrum sqrt(const Spectrum &s);
	friend Spectrum pow(const Spectrum &s, double p);
	friend Spectrum exp(const Spectrum &s);

	bool isBlack() const;
	bool hasNaN() const;

	double sum() const;
	double average() const;

	XYZ3 toXYZ3() const;
};

// @brief one sample point from a spectrum.
struct SpectrumSample
{
	double lambda;
	double value;

	SpectrumSample(double _lambda, double _value);

	// @brief sorted by lambda.
	bool operator>(const SpectrumSample &s) const;
};

// @brief The specturm samples uniformly. Actually used in program.
class SampledSpectrum
	: public Spectrum<nSpectrumSamples>
{
	// these spectrums should be calculated at compile time.
	// TODO
	static SampledSpectrum X, Y, Z;
	static SampledSpectrum rgbRefl2SpectWhite, rgbRefl2SpectCyan;
	static SampledSpectrum rgbRefl2SpectMagenta, rgbRefl2SpectYellow;
	static SampledSpectrum rgbRefl2SpectRed, rgbRefl2SpectGreen;
	static SampledSpectrum rgbRefl2SpectBlue;
	static SampledSpectrum rgbIllum2SpectWhite, rgbIllum2SpectCyan;
	static SampledSpectrum rgbIllum2SpectMagenta, rgbIllum2SpectYellow;
	static SampledSpectrum rgbIllum2SpectRed, rgbIllum2SpectGreen;
	static SampledSpectrum rgbIllum2SpectBlue;

public:
	friend class RGB3, XYZ3;

	SampledSpectrum();

	SampledSpectrum(double val);

	// @brief implicit construct from base type is allowed.
	SampledSpectrum(const Spectrum &s);

	// @brief generate SampledSpectrum from a set of SpectrumSample.
	static SampledSpectrum fromSampled(std::vector<SpectrumSample> v);
};

class RGBSpectrum : public Spectrum<3>
{
	// TODO RGBSpectrum
};