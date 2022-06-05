/**
 * @file Matrix.cpp
 * @author orbitchen
 * @brief Matrix impl.
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../Core/CoreLayer/Matrix.h"

Angle::Angle(double v, AngleType type)
{
	if (type == AngleType::ANGLE_DEG)
	{
		deg = v;
		rad = v / 180 * 3.14159;
	}
	else
	{
		rad = v;
		deg = v * 180 / 3.14159;
	}
}

double Angle::getDeg() const { return deg; }

double Angle::getRad() const { return rad; }

// Matrix4x4 impl

Matrix4x4::Matrix4x4(const Eigen::Matrix4d &_matrix)
{
	matrix = _matrix;
}

Matrix4x4::Matrix4x4()
{
	// empty
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &mat)
{
	return Matrix4x4(matrix * mat.matrix);
}

Vec3d Matrix4x4::operator*(const Vec3d &v)
{
	Eigen::Vector3d vec = Eigen::Vector3d(v[0], v[1], v[2]);
	Eigen::Vector3d result = matrix.block<3, 3>(0, 0) * vec;
	return Vec3d(result[0], result[1], result[2]);
}

Point3d Matrix4x4::operator*(const Point3d &p)
{
	Eigen::Vector4d vec = Eigen::Vector4d(p[0], p[1], p[2], 1.0f);
	Eigen::Vector4d result = matrix * vec;
	return Point3d(result[0] / result[3], result[1] / result[3], result[2] / result[3]);
}

Normal3d Matrix4x4::operator*(const Normal3d &n)
{
	Eigen::Vector3d vec = Eigen::Vector3d(n[0], n[1], n[2]);
	Eigen::Matrix3d mat = matrix.block<3, 3>(0, 0);
	// M_{normal}=(M_{point}^{-1})^T
	mat = mat.inverse().transpose();
	Eigen::Vector3d result = mat * vec;
	return Normal3d(result[0], result[1], result[2]);
}

Matrix4x4 Matrix4x4::translate(double x, double y, double z)
{
	Matrix4x4 retVal;
	retVal.matrix(0, 3) = x;
	retVal.matrix(1, 3) = y;
	retVal.matrix(2, 3) = z;
	return retVal;
}

Matrix4x4 Matrix4x4::scale(double x, double y, double z)
{
	Matrix4x4 retVal;
	retVal.matrix(0, 0) = x;
	retVal.matrix(1, 1) = y;
	retVal.matrix(2, 2) = z;
	return retVal;
}

Matrix4x4 Matrix4x4::scale(double ratio)
{
	Matrix4x4 retVal;
	retVal.matrix(0, 0) = ratio;
	retVal.matrix(1, 1) = ratio;
	retVal.matrix(2, 2) = ratio;
	return retVal;
}

Matrix4x4 Matrix4x4::rotateEuler(const Angle &x, const Angle &y, const Angle &z, EulerType type)
{
	Matrix4x4 retVal;
	Eigen::Matrix3d block;
	auto rotX = Eigen::AngleAxisd(x.getRad(), Eigen::Vector3d::UnitX());
	auto rotY = Eigen::AngleAxisd(y.getRad(), Eigen::Vector3d::UnitY());
	auto rotZ = Eigen::AngleAxisd(z.getRad(), Eigen::Vector3d::UnitZ());
	if (type == EulerType::EULER_XYZ)
	{
		block = rotZ * rotY * rotX;
	}
	else if (type == EulerType::EULER_ZYX)
	{
		block = rotX * rotY * rotZ;
	}
	else
	{
		// never should come here.
		// TODO report error.
	}
	retVal.matrix.block<3, 3>(0, 0) = block;
	return retVal;
}

Matrix4x4 Matrix4x4::rotateQuaternion(double w, double x, double y, double z)
{
	Eigen::Quaternion q = Eigen::Quaternion(w, x, y, z);
	Matrix4x4 retVal;
	retVal.matrix.block<3, 3>(0, 0) = q.matrix();
	return retVal;
}

Matrix4x4 Matrix4x4::rotateAxis(const Vec3d &axis, const Angle &angle)
{
	Matrix4x4 retVal;
	Eigen::Matrix3d block = Eigen::AngleAxisd(angle.getRad(), Eigen::Vector3d(axis[0], axis[1], axis[2])).matrix();
	retVal.matrix.block<3, 3>(0, 0) = block;
	return retVal;
}

// @brief camera coord: x->right y->up z->in
Matrix4x4 Matrix4x4::lookAt(const Point3d &lookFrom, const Vec3d &vecLookAt, const Vec3d &up)
{
	Matrix4x4 translateMat = translate(-lookFrom[0], -lookFrom[1], -lookFrom[2]);
	Vec3d realLookAt = normalize(vecLookAt);
	Vec3d right = normalize(cross(realLookAt, up));
	Vec3d realUp = normalize(cross(right, realLookAt));
	Matrix4x4 rotMat;
	// input manually
	rotMat.matrix << right[0], right[1], right[2], 0.0f, realUp[0], realUp[1], realUp[2], 0.0f,
		realLookAt[0], realLookAt[1], realLookAt[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f;
	return rotMat * translateMat;
}

Matrix4x4 Matrix4x4::orthographic(double left, double right, double up, double down, double near, double far)
{
	Matrix4x4 retVal;
	// input manually
	retVal.matrix << 2.0 / (right - left), 0.0f, 0.0f, -(right + left) / 2.0,
		0.0f, 2.0 / (up - down), 0.0f, -(up + down) / 2.0,
		0.0, 0.0, 2.0 / (far - near), -(far + near) / 2.0,
		0.0f, 0.0f, 0.0f, 1.0f;
	return retVal;
}

/*
 * @brief get persepctive projection matrix.
 * @param fov fov on x (left-right)
 * @param aspect x/y. e.g. 1920x1080 -> aspect=1920/1080
 * @param near near plane.
 * @param far far plane.
 */
Matrix4x4 Matrix4x4::perspective(const Angle &fov, double aspect, double near, double far)
{
	double cotHalfFov = (1 / std::tan(fov.getRad() / 2.0));
	Matrix4x4 retVal;
	retVal.matrix << cotHalfFov, 0.0f, 0.0f, 0.0f,
		0.0f, cotHalfFov / aspect, 0.0f, 0.0f,
		0.0f, 0.0f, (far + near) / (far - near), -2.0 * far * near / (far - near),
		0.0f, 0.0f, 1.0f, 0.0f;
	return retVal;
}

Matrix4x4 Matrix4x4::inverse()
{
	return Matrix4x4(matrix.inverse());
}

Matrix4x4 Matrix4x4::transpose()
{
	return Matrix4x4(matrix.transpose());
}

// Transform3D impl

void TransformMatrix3D::update()
{
	if (dirty)
	{
		dirty = false;
		// Scale->Rotate->Translate
		matrixAll = matrixTranslate * matrixRotate * matrixScale;
	}
}

TransformMatrix3D::TransformMatrix3D()
{
	dirty = true;
}

void TransformMatrix3D::setTranslate(double x, double y, double z)
{
	dirty = true;
	matrixTranslate = Matrix4x4::translate(x, y, z);
}

void TransformMatrix3D::setScale(double x, double y, double z)
{
	dirty = true;
	matrixScale = Matrix4x4::scale(x, y, z);
}

void TransformMatrix3D::setScale(double ratio)
{
	dirty = true;
	matrixScale = Matrix4x4::scale(ratio);
}

void TransformMatrix3D::setRotateEuler(const Angle &x, const Angle &y, const Angle &z, EulerType type)
{
	dirty = true;
	matrixRotate = Matrix4x4::rotateEuler(x, y, z, type);
}

void TransformMatrix3D::setRotateQuaternion(double w, double x, double y, double z)
{
	dirty = true;
	matrixRotate = Matrix4x4::rotateQuaternion(w, x, y, z);
}

void TransformMatrix3D::setRotateAxis(const Vec3d &axis, const Angle &angle)
{
	dirty = true;
	matrixRotate = Matrix4x4::rotateAxis(axis, angle);
}

Vec3d TransformMatrix3D::operator*(const Vec3d &v)
{
	update();
	return matrixAll * v;
}

Point3d TransformMatrix3D::operator*(const Point3d &p)
{
	update();
	return matrixAll * p;
}

Normal3d TransformMatrix3D::operator*(const Normal3d &n)
{
	update();
	return matrixAll * n;
}
