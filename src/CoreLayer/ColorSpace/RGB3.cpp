/**
 * @file Color.h
 * @author orbitchen
 * @brief RGB3 implemention.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "Color.h"

#include "Eigen/Dense"


RGB3::RGB3() 
{
	rgbData[0] = rgbData[1] = rgbData[2] = 0.0;
}

RGB3::RGB3(double r, double g, double b) 
{
	// no checking of [0,1] bounding.
	rgbData[0] = r;
	rgbData[1] = g;
	rgbData[2] = b;
}

RGB3::RGB3(double val) 
{
	rgbData[0] = rgbData[1] = rgbData[2] = val;
}

double RGB3::operator[](int i) const
{
	// no checking of i.
	return rgbData[i];
}

double& RGB3::operator[](int i)
{
	// no checking of i.
	return rgbData[i];
}

RGB3 RGB3::operator+(const RGB3& rgb)
{
	return RGB3(rgbData[0] + rgb[0], rgbData[1] + rgb[1], rgbData[2] + rgb[2]);
}

RGB3 RGB3::operator-(const RGB3& rgb)
{
	return RGB3(rgbData[0] - rgb[0], rgbData[0] - rgb[1], rgbData[0] - rgb[2]);
}

RGB3 RGB3::operator*(const RGB3& rgb)
{
	return RGB3(rgbData[0] * rgb[0], rgbData[0] * rgb[1], rgbData[0] * rgb[2]);
}

RGB3 RGB3::operator/(const RGB3& rgb)
{
	return RGB3(rgbData[0] / rgb[0], rgbData[0] / rgb[1], rgbData[0] / rgb[2]);
}

RGB3& RGB3::operator+=(const RGB3& rgb)
{
	rgbData[0] += rgb[0];
	rgbData[1] += rgb[1];
	rgbData[2] += rgb[2];
	return *this;
}

RGB3& RGB3::operator-=(const RGB3& rgb)
{
	rgbData[0] -= rgb[0];
	rgbData[1] -= rgb[1];
	rgbData[2] -= rgb[2];
	return *this;
}

RGB3& RGB3::operator*=(const RGB3& rgb)
{
	rgbData[0] *= rgb[0];
	rgbData[1] *= rgb[1];
	rgbData[2] *= rgb[2];
	return *this;
}

RGB3& RGB3::operator/=(const RGB3& rgb)
{
	rgbData[0] /= rgb[0];
	rgbData[1] /= rgb[1];
	rgbData[2] /= rgb[2];
	return *this;
}

RGB3 RGB3::operator*(double v)
{
	return RGB3(rgbData[0] * v, rgbData[1] * v, rgbData[2] * v);
}

RGB3 RGB3::operator/(double v)
{
	return RGB3(rgbData[0] / v, rgbData[1] / v, rgbData[2] / v);
}

RGB3& RGB3::operator*=(double v)
{
	rgbData[0] *= v;
	rgbData[1] *= v;
	rgbData[2] *= v;
	return *this;
}

RGB3& RGB3::operator/=(double v)
{
	rgbData[0] /= v;
	rgbData[1] /= v;
	rgbData[2] /= v;
	return *this;
}

XYZ3 RGB3::toXYZ3() const
{
	// TODO: Eigen impl maybe even slower.
	Eigen::Matrix3d rgb2xyzMatrix;
	rgb2xyzMatrix<< 0.412453f, 0.357580f, 0.180423f,
					0.212671f, 0.715160f, 0.072169f,
					0.019334f, 0.119193f, 0.950227f;
	Eigen::Vector3d vec;
	vec << rgbData[0], rgbData[1], rgbData[2];
	auto res = rgb2xyzMatrix * vec;
	return XYZ3(res[0], res[1], res[2]);
}

// @brief rgb to Spectrum. COSTLY so arrange cache if possible.
Spectrum RGB3::toSpectrum(SpectrumType type) const
{
    SampledSpectrum r;
    if (type == SpectrumType::REFLECTANCE) {
        // Convert reflectance spectrum to rgb
        if (rgbData[0] <= rgbData[1] && rgbData[0] <= rgbData[2]) {
            // Compute reflectance _SampledSpectrum_ with _rgb[0]_ as minimum
            r += rgbData[0] * SampledSpectrum::rgbRefl2SpectWhite;
            if (rgbData[1] <= rgbData[2]) {
                r += (rgbData[1] - rgbData[0]) * SampledSpectrum::rgbRefl2SpectCyan;
                r += (rgbData[2] - rgbData[1]) * SampledSpectrum::rgbRefl2SpectBlue;
            }
            else {
                r += (rgbData[2] - rgbData[0]) * SampledSpectrum::rgbRefl2SpectCyan;
                r += (rgbData[1] - rgbData[2]) * SampledSpectrum::rgbRefl2SpectGreen;
            }
        }
        else if (rgbData[1] <= rgbData[0] && rgbData[1] <= rgbData[2]) {
            // Compute reflectance _SampledSpectrum_ with _rgb[1]_ as minimum
            r += rgbData[1] * SampledSpectrum::rgbRefl2SpectWhite;
            if (rgbData[0] <= rgbData[2]) {
                r += (rgbData[0] - rgbData[1]) * SampledSpectrum::rgbRefl2SpectMagenta;
                r += (rgbData[2] - rgbData[0]) * SampledSpectrum::rgbRefl2SpectBlue;
            }
            else {
                r += (rgbData[2] - rgbData[1]) * SampledSpectrum::rgbRefl2SpectMagenta;
                r += (rgbData[0] - rgbData[2]) * SampledSpectrum::rgbRefl2SpectRed;
            }
        }
        else {
            // Compute reflectance _SampledSpectrum_ with _rgb[2]_ as minimum
            r += rgbData[2] * SampledSpectrum::rgbRefl2SpectWhite;
            if (rgbData[0] <= rgbData[1]) {
                r += (rgbData[0] - rgbData[2]) * SampledSpectrum::rgbRefl2SpectYellow;
                r += (rgbData[1] - rgbData[0]) * SampledSpectrum::rgbRefl2SpectGreen;
            }
            else {
                r += (rgbData[1] - rgbData[2]) * SampledSpectrum::rgbRefl2SpectYellow;
                r += (rgbData[0] - rgbData[1]) * SampledSpectrum::rgbRefl2SpectRed;
            }
        }
        r *= .94;
    }
    else {
        // Convert illuminant spectrum to rgbData
        if (rgbData[0] <= rgbData[1] && rgbData[0] <= rgbData[2]) {
            // Compute illuminant _SampledSpectrum_ with _rgb[0]_ as minimum
            r += rgbData[0] * SampledSpectrum::rgbIllum2SpectWhite;
            if (rgbData[1] <= rgbData[2]) {
                r += (rgbData[1] - rgbData[0]) * SampledSpectrum::rgbIllum2SpectCyan;
                r += (rgbData[2] - rgbData[1]) * SampledSpectrum::rgbIllum2SpectBlue;
            }
            else {
                r += (rgbData[2] - rgbData[0]) * SampledSpectrum::rgbIllum2SpectCyan;
                r += (rgbData[1] - rgbData[2]) * SampledSpectrum::rgbIllum2SpectGreen;
            }
        }
        else if (rgbData[1] <= rgbData[0] && rgbData[1] <= rgbData[2]) {
            // Compute illuminant _SampledSpectrum_ with _rgb[1]_ as minimum
            r += rgbData[1] * SampledSpectrum::rgbIllum2SpectWhite;
            if (rgbData[0] <= rgbData[2]) {
                r += (rgbData[0] - rgbData[1]) * SampledSpectrum::rgbIllum2SpectMagenta;
                r += (rgbData[2] - rgbData[0]) * SampledSpectrum::rgbIllum2SpectBlue;
            }
            else {
                r += (rgbData[2] - rgbData[1]) * SampledSpectrum::rgbIllum2SpectMagenta;
                r += (rgbData[0] - rgbData[2]) * SampledSpectrum::rgbIllum2SpectRed;
            }
        }
        else {
            // Compute illuminant _SampledSpectrum_ with _rgb[2]_ as minimum
            r += rgbData[2] * SampledSpectrum::rgbIllum2SpectWhite;
            if (rgbData[0] <= rgbData[1]) {
                r += (rgbData[0] - rgbData[2]) * SampledSpectrum::rgbIllum2SpectYellow;
                r += (rgbData[1] - rgbData[0]) * SampledSpectrum::rgbIllum2SpectGreen;
            }
            else {
                r += (rgbData[1] - rgbData[2]) * SampledSpectrum::rgbIllum2SpectYellow;
                r += (rgbData[0] - rgbData[1]) * SampledSpectrum::rgbIllum2SpectRed;
            }
        }
        r *= .86445f;
    }
    return r.clamp();
}

RGB3 operator*(double v, const RGB3& rgb)
{
	return RGB3(rgb[0] * v, rgb[1] * v, rgb[2] * v);
}
