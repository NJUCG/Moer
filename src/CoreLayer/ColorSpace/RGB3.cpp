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
#include "FastMath.h"

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

RGB3 RGB3::operator+(const RGB3& rgb) const
{
	return RGB3(rgbData[0] + rgb[0], rgbData[1] + rgb[1], rgbData[2] + rgb[2]);
}

RGB3 RGB3::operator-(const RGB3& rgb) const
{
	return RGB3(rgbData[0] - rgb[0], rgbData[1] - rgb[1], rgbData[2] - rgb[2]);
}

RGB3 RGB3::operator*(const RGB3& rgb) const
{
	return RGB3(rgbData[0] * rgb[0], rgbData[1] * rgb[1], rgbData[2] * rgb[2]);
}

RGB3 RGB3::operator/(const RGB3& rgb) const
{
	return RGB3(rgbData[0] / rgb[0], rgbData[1] / rgb[1], rgbData[2] / rgb[2]);
}

RGB3  RGB3::pow(double v) const
{
    return RGB3(fm::pow(rgbData[0],v),fm::pow(rgbData[1],v),fm::pow(rgbData[2],v));
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



RGB3 operator*(double v, const RGB3& rgb)
{
	return RGB3(rgb[0] * v, rgb[1] * v, rgb[2] * v);
}



