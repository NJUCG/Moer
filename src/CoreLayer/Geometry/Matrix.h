/**
 * @file Matrix.h
 * @author orbitchen
 * @brief Matrix4x4 (Eigen backend) and TransformMatrix3D.
 * @version 0.2
 * @date 2022-05-10
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "Geometry.h"

#include "Eigen/Dense"

 /*
  * @brief Angle type. Providing convenient angle transformation between deg and rad.
  */
class Angle
{
	double deg;
	double rad;

public:
	enum class AngleType
	{
		ANGLE_DEG,
		ANGLE_RAD
	};
	/*
	 * @brief init an angle.
	 * @param v value of angle, deg or rad.
	 * @param type the type of v.
	 */
	Angle(double v, AngleType type);

	double getDeg() const;

	double getRad() const;
};

enum class EulerType
{
	EULER_XYZ,
	EULER_ZYX
};

// @brief a simple wrap of Eigen Matrix 4x4.
class Matrix4x4
{
private:

	// @brief Eigen data. inaccessible from outside.
	Eigen::Matrix4d matrix=Eigen::Matrix4d::Identity();

	// @brief init from Eigen data. inaccessible from outside.
	Matrix4x4(const Eigen::Matrix4d& _matrix);

public:

	Matrix4x4();

	Matrix4x4 operator*(const Matrix4x4& mat) const;

	Vec3d operator*(const Vec3d& v) const;
	Point3d operator*(const Point3d& p) const;
	Normal3d operator*(const Normal3d& n) const;

	// model
	static Matrix4x4 translate(double x, double y, double z);
	static Matrix4x4 scale(double x, double y, double z);
	static Matrix4x4 scale(double ratio);
	static Matrix4x4 rotateEuler(const Angle& x, const Angle& y, const Angle& z, EulerType type = EulerType::EULER_XYZ);
	static Matrix4x4 rotateQuaternion(double w, double x, double y, double z);
	static Matrix4x4 rotateAxis(const Vec3d& axis, const Angle& angle);

	// view & projection
	static Matrix4x4 lookAt(const Point3d& lookFrom, const Vec3d& vecLookAt, const Vec3d& up);
	static Matrix4x4 orthographic(double left,double right,double up,double down,double near,double far);
	static Matrix4x4 perspective(const Angle& fov,double aspect, double near, double far);

	Matrix4x4 inverse();
	Matrix4x4 transpose();

	friend void printMatrix(const Matrix4x4 &mat);
};

/*
 * @brief Encapsulated transform matrix. By default, it will be initialized as identity matrix.
 */
class TransformMatrix3D
{
	// @brief the matrix that applies rotate, scale and translate.
	Matrix4x4 matrixAll;

	Matrix4x4 matrixRotate;
	Matrix4x4 matrixScale;
	Matrix4x4 matrixTranslate;

	// @brief true iff matrixAll!=matrixTranslate*matrixScale*matrixRotate.
	bool dirty;

	void update();

public:
	TransformMatrix3D();

	void setTranslate(double x, double y, double z);

	void setScale(double x, double y, double z);
	void setScale(double ratio);

	void setRotateEuler(const Angle& x, const Angle& y, const Angle& z, EulerType type = EulerType::EULER_XYZ);

	void setRotateQuaternion(double w, double x, double y, double z);

	// @brief Rotate by axis. Counterclockwise rotate.
	void setRotateAxis(const Vec3d& axis, const Angle& angle);

	Vec3d operator*(const Vec3d &v);
	Point3d operator*(const Point3d &p);
	Normal3d operator*(const Normal3d &n);
};

// TODO TransformMatrix2D