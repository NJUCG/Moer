/**
 * @file Matrix.h
 * @author orbitchen
 * @brief Transform matrix for the whole program, including 2D transform matrix and 3D transform matrix. Using Eigen as matrix backend.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Geometry.h"

#include "Eigen/Dense"

/*
* @brief Angle type. Providing convenient angle transformation between deg and rad.
*/
class Angle {
	double deg;
	double rad;

public:
	enum class AngleType {ANGLE_DEG,ANGLE_RAD};
	/*
	* @brief init an angle.
	* @param v value of angle, deg or rad.
	* @param type the type of v.
	*/
	Angle(double v, AngleType type);

	getDeg() const;

	getRad() const;

};
/*
* @brief Encapsulated transform matrix. By default, it will be initialized as identity matrix.
*/
class TransformMatrix3D
{
	// @brief the matrix that applies rotate, scale and translate.
	Eigen::Matrix4d matrixAll;

	Eigen::Matrix4d matrixRotate;
	Eigen::Matrix4d matrixScale;
	Eigen::Matrix4d matrixTranslate;

	// @brief true iff matrixAll!=matrixTranslate*matrixScale*matrixRotate.
	bool dirty;

public:

	TransformMatrix3D();

	void setTranslate(double x, double y, double z);

	void setScale(double x, double y, double z);
	void setScale(double ratio);

	enum class EulerType {EULER_XYZ,EULER_ZYX};
	void setRotateEuler(Angle x, Angle y, Angle z, EulerType type = EulerType::EULER_XYZ);

	void setRotateQuaternion(double w, double x, double y, double z);

	// @brief Rotate by axis. Counterclockwise rotate.
	void setRotateAxis(Angle angle, Vec3f axis);

	Vec3f operator*(const Vec3f& v);
	Point3f operator*(const Point3f& p);
	Normal3f operator*(const Normal3f& n);
};

// TODO TransformMatrix2D