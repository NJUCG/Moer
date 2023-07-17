/**
 * @file Color.h
 * @author orbitchen
 * @brief Color representation, including rgb, xyz and spectrum.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */
#pragma once

#include <vector>
#include <cfloat>
#include "CoreLayer/Math/Common.h"
#include "FastMath.h"

class RGB3;
class XYZ3;
class SampledSpectrum;
class RGBSpectrum;

// TODO: should be defined by cmake marco.

#ifdef USING_SAMPLED_SPECTRUM
using Spectrum = SampledSpectrum;
#else
using Spectrum = RGBSpectrum;
#endif

static const double sampledLambdaStart = 400.0;
static const double sampledLambdaEnd = 700.0;

// The number of uniform samples for SampledSpectrum.
#define SPECTRUM_SAMPLES CMAKE_DEF_SPECTRUM_SAMPLES
#ifdef USING_SAMPLED_SPECTRUM
static const int nSpectrumSamples = SPECTRUM_SAMPLES;
#else
static const int nSpectrumSamples = 3;
#endif

/// \brief types of spectrum. different strategies will be applied.
enum class SpectrumType { REFLECTANCE, ILLUMINANT };


/**
 * @brief Classical rgb color space
 * 
 */
class RGB3
{
private:
	double rgbData[3];

public:
    static void init(){}

    RGB3();

	RGB3(double r, double g, double b);

	// @brief initialize all rgb value as val.
	RGB3(double val);

	double operator[](int i) const;
	double &operator[](int i);

	RGB3 operator+(const RGB3 &rgb) const ;
	RGB3 operator-(const RGB3 &rgb) const ;
	RGB3 operator*(const RGB3 &rgb) const ;
	RGB3 operator/(const RGB3 &rgb) const ;

    RGB3  pow(double v) const ;

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

};


/**
 * @brief XYZ3 color space
 * 
 */
class XYZ3
{
	double xyzData[3];

public:
	XYZ3();

	XYZ3(double x, double y, double z);

	// @brief initialize all xyz value as val.
	XYZ3(double val);

	double operator[](int i) const;
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

	/// @brief Convert XYZ3 to RGB3
	RGB3 toRGB3() const;
};

/**
 * 	@brief Spectrum of nSamples sample points.
 * 	@tparam nSamples The number of samples
 */
template <int nSamples>
class CoefficientSpectrum
{
protected:
	double coefficients[nSamples];

public:
	/// @brief All coefficients initialized as 0.0f.
	CoefficientSpectrum() {
		for (int i = 0; i < nSamples; i++) {
			coefficients[i] = 0.0;
		}
	}

	/// @brief All coefficients initialized as val.
	CoefficientSpectrum(double val) {
		for (int i = 0; i < nSamples; i++) {
			coefficients[i] = val;
		}
	}

	/// @brief Access the value of the spectrum at the ith sample point
	/// @attention No bounding check
	double operator[](int i) const {
		return coefficients[i];
	}

	/// @brief Access the value of the spectrum at the ith sample point
	/// @attention  No bounding check
	double &operator[](int i) {
		return coefficients[i];
	}

	/// @brief Each component of the two spectra is added correspondingly
	CoefficientSpectrum operator+(const CoefficientSpectrum&s) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] += s.coefficients[i];
		}
		return retVal;
	}

	/// @brief Subtract each component of the two spectra
	CoefficientSpectrum operator-(const CoefficientSpectrum&s) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] -= s.coefficients[i];
		}
		return retVal;
	}

	/// @brief Each component of the two spectra is multiplied correspondingly
	CoefficientSpectrum operator*(const CoefficientSpectrum&s) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] *= s.coefficients[i];
		}
		return retVal;
	}

	/// @brief Each component of the spectra is divided by the corresponding component of the other spectra
	/// @attention There may be NaNs in result
	CoefficientSpectrum operator/(const CoefficientSpectrum&s) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] /= s.coefficients[i];	// NaN
		}
		return retVal;
	}

	/// @brief Each component of the two spectra is added correspondingly
	CoefficientSpectrum& operator+=(const CoefficientSpectrum&s) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] += s.coefficients[i];
		}
		return *this;
	}

	/// @brief Subtract each component of the two spectra
	CoefficientSpectrum& operator-=(const CoefficientSpectrum&s) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] -= s.coefficients[i];
		}
		return *this;
	}

	/// @brief Each component of the two spectra is multiplied correspondingly
	CoefficientSpectrum& operator*=(const CoefficientSpectrum&s) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] *= s.coefficients[i];
		}
		return *this;
	}

	/// @attention There may be NaNs in result
	CoefficientSpectrum& operator/=(const CoefficientSpectrum&s) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] /= s.coefficients[i];	// NaN
		}
		return *this;
	}

	/// @brief Scale each component of the spectra (multiple by v)
	CoefficientSpectrum operator*(double v) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] *= v;
		}
		return retVal;
	}

	/// @brief Scale each component of the spectra (divide v)
	/// @attention There may be NaNs in result
	CoefficientSpectrum operator/(double v) const {
		CoefficientSpectrum retVal = *this;
		for (int i = 0; i < nSamples; i++) {
			retVal.coefficients[i] /= v;	// NaN
		}
		return retVal;
	}

	/// @brief Scale each component of the spectra (multiple by v)
	CoefficientSpectrum& operator*=(double v) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] *= v;
		}
		return *this;
	}

	/// @brief Scale each component of the spectra (divide v)
	/// @attention There may be NaNs in result
	CoefficientSpectrum& operator/=(double v) {
		for (int i = 0; i < nSamples; i++) {
			this->coefficients[i] /= v;	// NaN
		}
		return *this;
	}

	/// @brief Scale each component of the spectra (multiple by v)
	friend CoefficientSpectrum operator*(double v, const CoefficientSpectrum& s) {
		return s * v;
	}

	/// @brief Call fm::sqrt() on each component 
	/// @attention Does not check whether the value on each component is greater than zero
	friend CoefficientSpectrum sqrt(const CoefficientSpectrum& s) {
		CoefficientSpectrum ret;
		for (int i = 0; i < nSamples; i++)
			ret[i] = fm::sqrt(s[i]);
		return ret;
	}

	/// @brief Call fm::pow() on each component
	friend CoefficientSpectrum pow(const CoefficientSpectrum&s, double e) {
		CoefficientSpectrum ret;
		for (int i = 0; i < nSamples; i++)
			ret[i] = fm::pow(s[i], e);
		return ret;
	}

	/// @brief Call fm::exp() on each component
	friend CoefficientSpectrum exp(const CoefficientSpectrum&s) {
		CoefficientSpectrum ret;
		for (int i = 0; i < nSamples; i++)
			ret[i] = fm::exp(s[i]);
		return ret;
	}

	/// @brief Returns true if all components are not equal to zero
	bool isBlack() const {
		for (int i = 0; i < nSamples; i++) {
			if (coefficients[i] != 0.0) 
				return false;
		}
		return true;
	}

	/// @brief Returns true if any component is NaN
	bool hasNaN() const {
		for (int i = 0; i <nSamples; i++)
			if (std::isnan(coefficients[i]))
				return true;
		return false;
	}

	/// @brief Clamp each component of the spectra to a given range
	/// @param low Lower bound of the range (default 0)
	/// @param high Upper bound of the range (default max of double)
	inline CoefficientSpectrum clamp(double low = 0.0, double high = DBL_MAX) const {
		CoefficientSpectrum retVal;
		for (int i = 0; i < nSamples; i++) {
			retVal[i] = (coefficients[i]>low)?(coefficients[i]):(low);
			retVal[i] = (coefficients[i]<high)?(retVal[i]):(high);
		}
		return retVal;
	}

	/// @brief Returns the summary of each component
	double sum() const {
		double sum = 0;
		for (int i = 0; i < nSamples; i++) {
			sum += coefficients[i];
		}
		return sum;
	}

	/// @brief Returns the average of each component
	double average() const {
		return sum() / nSamples;
	}


	/// \attention This function is just used for debugging
	virtual XYZ3 toXYZ3() const {
		// this function should never be called.
		return XYZ3(0.0);
	}

	/// \attention This function is just used for debugging
	virtual RGB3 toRGB3() const {
		// this function should never be called.
		return RGB3(0.0);
	}

     /// @brief get the luminance
     virtual double luminance() const {
        return 0;
    }
};

/// \brief One sample point from a spectrum.
struct SpectrumSample
{
	double lambda;
	double value;

	SpectrumSample(double _lambda, double _value) {
		lambda = _lambda;
		value = _value;
	}

	/// \brief Sorted by lambda.
	bool operator>(const SpectrumSample& s) const {
		return lambda > s.lambda;
	}

	/// \brief Sorted by lambda.
	bool operator<(const SpectrumSample& s) const {
		return lambda < s.lambda;
	}
};

/// \brief The specturm samples uniformly. Actually used in program.
class SampledSpectrum
	: public CoefficientSpectrum<nSpectrumSamples>
{
	// these spectrums should be calculated at compile time.
	static SampledSpectrum X;
	static SampledSpectrum Y;
	static SampledSpectrum Z;

	static SampledSpectrum rgbRefl2SpectWhite;
	static SampledSpectrum rgbRefl2SpectCyan;
	static SampledSpectrum rgbRefl2SpectMagenta;
	static SampledSpectrum rgbRefl2SpectYellow;
	static SampledSpectrum rgbRefl2SpectRed;
	static SampledSpectrum rgbRefl2SpectGreen;
	static SampledSpectrum rgbRefl2SpectBlue;

	static SampledSpectrum rgbIllum2SpectWhite;
	static SampledSpectrum rgbIllum2SpectCyan;
	static SampledSpectrum rgbIllum2SpectMagenta;
	static SampledSpectrum rgbIllum2SpectYellow;
	static SampledSpectrum rgbIllum2SpectRed;
	static SampledSpectrum rgbIllum2SpectGreen;
	static SampledSpectrum rgbIllum2SpectBlue;

public:
	friend class RGB3;
	friend class XYZ3;

	/// \brief Global init of static values. should be called before any constructor of SampledSpectrum.
	static void init();

	SampledSpectrum();

	SampledSpectrum(double val);

	SampledSpectrum(const RGB3& rgb,SpectrumType type=SpectrumType::REFLECTANCE);

	SampledSpectrum(const CoefficientSpectrum& s);

	/// \brief generate SampledSpectrum from a set of SpectrumSample.
	static SampledSpectrum fromSampled(std::vector<SpectrumSample> v);

	virtual XYZ3 toXYZ3() const override;

	virtual RGB3 toRGB3() const override;

    virtual double luminance() const override;
};

/// @brief RGB spectrum. The value of RGB spectrum is the same as RGB3.
class RGBSpectrum : public CoefficientSpectrum<3>
{
public:
    static void init() {}


    RGBSpectrum();

	RGBSpectrum(double val);

	RGBSpectrum(const RGB3& rgb);

	RGBSpectrum(const CoefficientSpectrum& s);

	static RGBSpectrum fromSampled(std::vector<SpectrumSample> v,int n);

	virtual XYZ3 toXYZ3() const override;

	virtual RGB3 toRGB3() const override;

    virtual double luminance() const override;
};

