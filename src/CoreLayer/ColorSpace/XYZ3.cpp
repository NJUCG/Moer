/**
 * @file Color.h
 * @author orbitchen
 * @brief XYZ3 implemention.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */

#include "Color.h"
#include "Eigen/Dense"

XYZ3::XYZ3()
{
	xyzData[0] = xyzData[1] = xyzData[2] = 0.0;
}

XYZ3::XYZ3(double r, double g, double b)
{
	// no checking of [0,1] bounding.
	xyzData[0] = r;
	xyzData[1] = g;
	xyzData[2] = b;
}

XYZ3::XYZ3(double val)
{
	xyzData[0] = xyzData[1] = xyzData[2] = val;
}

double XYZ3::operator[](int i) const
{
	// no checking of i.
	return xyzData[i];
}

double& XYZ3::operator[](int i)
{
	// no checking of i.
	return xyzData[i];
}

XYZ3 XYZ3::operator+(const XYZ3& xyz)
{
	return XYZ3(xyzData[0] + xyz[0], xyzData[1] + xyz[1], xyzData[2] + xyz[2]);
}

XYZ3 XYZ3::operator-(const XYZ3& xyz)
{
	return XYZ3(xyzData[0] - xyz[0], xyzData[0] - xyz[1], xyzData[0] - xyz[2]);
}

XYZ3 XYZ3::operator*(const XYZ3& xyz)
{
	return XYZ3(xyzData[0] * xyz[0], xyzData[0] * xyz[1], xyzData[0] * xyz[2]);
}

XYZ3 XYZ3::operator/(const XYZ3& xyz)
{
	return XYZ3(xyzData[0] / xyz[0], xyzData[0] / xyz[1], xyzData[0] / xyz[2]);
}

XYZ3& XYZ3::operator+=(const XYZ3& xyz)
{
	xyzData[0] += xyz[0];
	xyzData[1] += xyz[1];
	xyzData[2] += xyz[2];
	return *this;
}

XYZ3& XYZ3::operator-=(const XYZ3& xyz)
{
	xyzData[0] -= xyz[0];
	xyzData[1] -= xyz[1];
	xyzData[2] -= xyz[2];
	return *this;
}

XYZ3& XYZ3::operator*=(const XYZ3& xyz)
{
	xyzData[0] *= xyz[0];
	xyzData[1] *= xyz[1];
	xyzData[2] *= xyz[2];
	return *this;
}

XYZ3& XYZ3::operator/=(const XYZ3& xyz)
{
	xyzData[0] /= xyz[0];
	xyzData[1] /= xyz[1];
	xyzData[2] /= xyz[2];
	return *this;
}

XYZ3 XYZ3::operator*(double v)
{
	return XYZ3(xyzData[0] * v, xyzData[1] * v, xyzData[2] * v);
}

XYZ3 XYZ3::operator/(double v)
{
	return XYZ3(xyzData[0] / v, xyzData[1] / v, xyzData[2] / v);
}

XYZ3& XYZ3::operator*=(double v)
{
	xyzData[0] *= v;
	xyzData[1] *= v;
	xyzData[2] *= v;
	return *this;
}

XYZ3& XYZ3::operator/=(double v)
{
	xyzData[0] /= v;
	xyzData[1] /= v;
	xyzData[2] /= v;
	return *this;
}

RGB3 XYZ3::toRGB3() const
{
	// TODO: Eigen impl maybe even slower.
	Eigen::Matrix3d rgb2xyzMatrix;
	rgb2xyzMatrix <<3.240479f, -1.537150f, -0.498535f,
					-0.969256f, 1.875991f, 0.041556f,
					0.055648f, -0.204043f, 1.057311f;
	Eigen::Vector3d vec;
	vec << xyzData[0], xyzData[1], xyzData[2];
	auto res = rgb2xyzMatrix * vec;
	return RGB3(res[0], res[1], res[2]);
}

XYZ3 operator*(double v, const XYZ3& xyz)
{
	return XYZ3(xyz[0] * v, xyz[1] * v, xyz[2] * v);
}

