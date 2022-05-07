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
#include <cmath>
#include <cfloat>

class RGB3;
class XYZ3;
class Spectrum;

// @brief the number of uniform samples for SampledSpectrum.
static const int nSpectrumSamples = 60;

// @brief mathematical clamp.
double clamp(double source, double low=0.0, double high=DBL_MAX);

// @brief mathematical lerp.
double lerp(double source0,double source1,double ratio);

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
class CoefficientSpectrum
{
protected:
	double coefficients[nSamples];

public:
	// @brief all coefficients initialized as 0.0f.
	CoeffieientSpectrum() {
		for (int i = 0; i < nSamples; i++) {
			coefficients[i] = 0.0;
		}
	}

	// @brief all coefficients initialized as val.
	CoeffieientSpectrum(double val) {
		for (int i = 0; i < nSamples; i++) {
			coefficients[i] = initVal;
		}
	}

	double operator[](int i) {
		// no bounding check.
		return coefficients[i];
	}

	double &operator[](int i) {
		// no bounding check.
		return coefficients[i];
	}

	CoefficientSpectrum operator+(const CoefficientSpectrum&s) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] += s.coefficients[i];
		}
		return retVal;
	}

	CoefficientSpectrum operator-(const CoefficientSpectrum&s) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] -= s.coefficients[i];
		}
		return retVal;
	}

	CoefficientSpectrum operator*(const CoefficientSpectrum&s) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] *= s.coefficients[i];
		}
		return retVal;
	}

	CoefficientSpectrum operator/(const CoefficientSpectrum&s) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] /= s.coefficients[i];	// NaN
		}
		return retVal;
	}

	CoefficientSpectrum& operator+=(const CoefficientSpectrum&s) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] += s.coefficients[i];
		}
		return *this;
	}

	CoefficientSpectrum& operator-=(const CoefficientSpectrum&s) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] -= s.coefficients[i];
		}
		return *this;
	}

	CoefficientSpectrum& operator*=(const CoefficientSpectrum&s) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] *= s.coefficients[i];
		}
		return *this;
	}

	CoefficientSpectrum& operator/=(const CoefficientSpectrum&s) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] /= s.coefficients[i];	// NaN
		}
		return *this;
	}

	CoefficientSpectrum operator*(double v) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] *= v;
		}
		return retVal;
	}

	CoefficientSpectrum operator/(double v) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] /= v;	// NaN
		}
		return retVal;
	}

	CoefficientSpectrum& operator*=(double v) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] *= v;
		}
		return *this;
	}

	CoefficientSpectrum& operator/=(double v) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] /= v;	// NaN
		}
		return *this;
	}

	friend CoefficientSpectrum operator*(double v, const CoefficientSpectrum& s) {
		return s * v;
	}

	friend CoefficientSpectrum sqrt(const CoefficientSpectrum& s) {
		CoefficientSpectrum ret;
		for (int i = 0; i < nSamples; i++)
			ret[i] = std::sqrt(s[i]);
		return ret;
	}

	friend CoefficientSpectrum pow(const CoefficientSpectrum&s, double e) {
		CoefficientSpectrum ret;
		for (int i = 0; i < nSamples; i++)
			ret[i] = std::pow(s[i], e);
		return ret;
	}

	friend CoefficientSpectrum exp(const CoefficientSpectrum&s) {
		CoefficientSpectrum ret;
		for (int i = 0; i < nSamples; i++)
			ret[i] = std::exp(s[i]);
		return ret;
	}

	bool isBlack() const {
		for (int i = 0; i < nSamples; i++) {
			if (coefficients[i] != 0.0) 
				return false;
		}
	}

	bool hasNaN() const {
		for (int i = 0; i <nSamples; i++)
			if (std::isnan(coefficients[i]))
				return true;
		return false;
	}

	inline CoefficientSpectrum clamp(double low = 0.0, double high = DBL_MAX) const {
		CoefficientSpectrum retVal;
		for (int i = 0; i < nSamples; i++) {
			retVal[i] = clamp(coefficients[i], low, high);
		}
		return retVal;
	}

	double sum() const {
		double sum = 0;
		for (int i = 0; i < nSamples; i++) {
			sum += coefficients[i];
		}
		return sum;
	}

	double average() const {
		return sum() / nSamples;
	}

	virtual XYZ3 toXYZ3() const {
		//DEBUG this function should never be called.
	}
};

// @brief one sample point from a spectrum.
struct SpectrumSample
{
	double lambda;
	double value;

	SpectrumSample(double _lambda, double _value) {
		lambda = _lambda;
		value = _value;
	}

	// @brief sorted by lambda.
	bool operator>(const SpectrumSample& s) const {
		return lambda > s.lambda;
	}
};

// extra function declaration
constexpr double averageSpectrumSamples(std::vector<SpectrumSample> samples, double lambdaBegin, double lambdaEnd);
constexpr std::vector<SpectrumSample> constructSpectrumSamples(const double* lambda, const double* value, const int n);

// constant declaration
// TODO should't be included by core.
const int nCIESamples = 471;
extern const double CIE_Y_integral = 106.856895;
extern const double CIE_X[nCIESamples];
extern const double CIE_Y[nCIESamples];
extern const double CIE_Z[nCIESamples];
extern const double CIE_lambda[nCIESamples];

static const int nRGB2SpectSamples = 32;
extern const double RGB2SpectLambda[nRGB2SpectSamples];
extern const double RGBRefl2SpectWhite[nRGB2SpectSamples];
extern const double RGBRefl2SpectCyan[nRGB2SpectSamples];
extern const double RGBRefl2SpectMagenta[nRGB2SpectSamples];
extern const double RGBRefl2SpectYellow[nRGB2SpectSamples];
extern const double RGBRefl2SpectRed[nRGB2SpectSamples];
extern const double RGBRefl2SpectGreen[nRGB2SpectSamples];
extern const double RGBRefl2SpectBlue[nRGB2SpectSamples];
extern const double RGBIllum2SpectWhite[nRGB2SpectSamples];
extern const double RGBIllum2SpectCyan[nRGB2SpectSamples];
extern const double RGBIllum2SpectMagenta[nRGB2SpectSamples];
extern const double RGBIllum2SpectYellow[nRGB2SpectSamples];
extern const double RGBIllum2SpectRed[nRGB2SpectSamples];
extern const double RGBIllum2SpectGreen[nRGB2SpectSamples];
extern const double RGBIllum2SpectBlue[nRGB2SpectSamples];

// @brief The specturm samples uniformly. Actually used in program.
class SampledSpectrum
	: public CoefficientSpectrum<nSpectrumSamples>
{
	// these spectrums should be calculated at compile time.
	static constexpr SampledSpectrum X = fromSampled(constructSpectrumSamples(CIE_lambda, CIE_X, nCIESamples));
	static constexpr SampledSpectrum Y = fromSampled(constructSpectrumSamples(CIE_lambda, CIE_Y, nCIESamples));
	static constexpr SampledSpectrum Z = fromSampled(constructSpectrumSamples(CIE_lambda, CIE_Z, nCIESamples));

	static constexpr SampledSpectrum rgbRefl2SpectWhite = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectWhite, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbRefl2SpectCyan = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectCyan, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbRefl2SpectMagenta = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectMagenta, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbRefl2SpectYellow = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectYellow, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbRefl2SpectRed = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectRed, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbRefl2SpectGreen = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectGreen, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbRefl2SpectBlue = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectBlue, nRGB2SpectSamples));

	static constexpr SampledSpectrum rgbIllum2SpectWhite = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectWhite, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbIllum2SpectCyan = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectCyan, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbIllum2SpectMagenta = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectMagenta, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbIllum2SpectYellow = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectYellow, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbIllum2SpectRed = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectRed, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbIllum2SpectGreen = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectGreen, nRGB2SpectSamples));
	static constexpr SampledSpectrum rgbIllum2SpectBlue = fromSampled(constructSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectBlue, nRGB2SpectSamples));

public:
	friend class RGB3;
	friend class XYZ3;

	SampledSpectrum();

	SampledSpectrum(double val);

	// @brief generate SampledSpectrum from a set of SpectrumSample.
	static constexpr SampledSpectrum fromSampled(std::vector<SpectrumSample> v);

	virtual toXYZ3() const override;
};

class RGBSpectrum : public Spectrum<3>
{
	// TODO RGBSpectrum
};

// TODO: should be defined by cmake marco.
using Spectrum = SampledSpectrum;