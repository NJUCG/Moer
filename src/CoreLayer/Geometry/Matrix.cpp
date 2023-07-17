/**
 * @file Matrix.cpp
 * @author orbitchen
 * @brief Matrix impl.
 * @version 0.1
 * @date 2022-05-10
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#include "Matrix.h"
#include "FastMath.h"

// Matrix4x4 impl

Matrix4x4::Matrix4x4(const Eigen::Matrix4d &_matrix)
{
	matrix = _matrix;
}

Matrix4x4::Matrix4x4()
{
	// empty
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &mat) const
{
	return Matrix4x4(matrix * mat.matrix);
}

Vec3d Matrix4x4::operator*(const Vec3d &v) const
{
	Eigen::Vector3d vec = Eigen::Vector3d(v[0], v[1], v[2]);
	Eigen::Vector3d result = matrix.block<3, 3>(0, 0) * vec;
	return Vec3d(result[0], result[1], result[2]);
}

Point3d Matrix4x4::operator*(const Point3d &p) const
{
	Eigen::Vector4d vec = Eigen::Vector4d(p[0], p[1], p[2], 1.0f);
	Eigen::Vector4d result = matrix * vec;
	return Point3d(result[0] / result[3], result[1] / result[3], result[2] / result[3]);
}

Normal3d Matrix4x4::operator*(const Normal3d &n) const
{
	Eigen::Vector3d vec = Eigen::Vector3d(n[0], n[1], n[2]);
	Eigen::Matrix3d mat = matrix.block<3, 3>(0, 0);
	// M_{normal}=(M_{point}^{-1})^T
	mat = mat.inverse().transpose();
	Eigen::Vector3d result = mat * vec;
	return Normal3d(result[0], result[1], result[2]);
}

Eigen::MatrixXd Matrix4x4::transformPoints(const Eigen::MatrixXd & points) const {
    Eigen::Matrix3d mat = matrix.block<3, 3>(0, 0);
    Eigen::Vector3d translateVector= matrix.block<3,1>(0,3);
    Eigen::MatrixXd transformPoints= mat*points;
   // return mat *(points.colwise() + translateVector);
    return  transformPoints.colwise() + translateVector;
}

Eigen::MatrixXd Matrix4x4::transformNormals(const Eigen::MatrixXd & normals) const {
    Eigen::Matrix3d mat = matrix.block<3, 3>(0, 0);
    Eigen::Matrix3d scale  = Matrix4x4::scale(1.0/mat.col(0).norm(),1.0/mat.col(1).norm(),1.0/mat.col(2).norm()).
            matrix.block<3, 3>(0, 0);
    return (scale * mat) * normals;
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
    else if (type == EulerType::EULER_YZX)
    {
        block = rotX * rotZ * rotY;
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

/// @brief camera coord: x->right y->up z->in
Matrix4x4 Matrix4x4::lookAt(const Point3d &lookFrom, const Vec3d &vecLookAt, const Vec3d &up)
{
	Matrix4x4 translateMat = translate(-lookFrom[0], -lookFrom[1], -lookFrom[2]);
	Vec3d realLookAt = normalize(vecLookAt);
	Vec3d right = normalize(cross(up, realLookAt));
	Vec3d realUp = normalize(cross(realLookAt, right));
	Matrix4x4 rotMat;
	// input manually
    //todo ugly now
	rotMat.matrix << -right[0], -right[1], -right[2], 0.0f, realUp[0], realUp[1], realUp[2], 0.0f,
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

/**
 * @brief get persepctive projection matrix.
 * @param fov fov on x (left-right)
 * @param aspect x/y. e.g. 1920x1080 -> aspect=1920/1080
 * @param near near plane.
 * @param far far plane.
 */
Matrix4x4 Matrix4x4::perspective(const Angle &fov, double aspect, double near, double far)
{
	/*
	double cotHalfFov = (1 / fm::tan(fov.getRad() / 2.0));
	Matrix4x4 retVal;
	retVal.matrix << cotHalfFov, 0.0f, 0.0f, 0.0f,
		0.0f, cotHalfFov / aspect, 0.0f, 0.0f,
		0.0f, 0.0f, (far + near) / (far - near), -2.0 * far * near / (far - near),
		0.0f, 0.0f, 1.0f, 0.0f;
	return retVal;
	*/
	// zcx 6.10
	double cotHalfFov = (1 / fm::tan(fov.getRad() / 2.0));
	Matrix4x4 retVal;
	retVal.matrix << cotHalfFov, 0, 0, 0,
		      		 0, cotHalfFov * aspect, 0, 0,
			  		 0, 0, far/(far-near), -far*near/(far-near),
			  		 0, 0, 1, 0;
	return retVal;
}

Matrix4x4 Matrix4x4::inverse() const
{
	return Matrix4x4(matrix.inverse());
}

Matrix4x4 Matrix4x4::transpose() const
{
	return Matrix4x4(matrix.transpose());
}

Matrix4x4::Matrix4x4(const double * matData) {
    memcpy(matrix.data(),matData,16*sizeof(double));
}
double *Matrix4x4::getTransformData() {
    return matrix.data();
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

TransformMatrix3D::TransformMatrix3D(const double * transformData) {
    matrixAll = Matrix4x4(transformData);
    dirty = false;
}

Eigen::MatrixXd TransformMatrix3D::transformPoints(const Eigen::MatrixXd & points) {
    update();
    return matrixAll.transformPoints(points);
}

Eigen::MatrixXd TransformMatrix3D::transformNormals(const Eigen::MatrixXd & normals) {
    update();
    return matrixAll.transformNormals(normals);
}

Matrix4x4 TransformMatrix3D::getScale() const 
{
	return matrixScale;
}

Matrix4x4 TransformMatrix3D::getRotate() const 
{
	return matrixRotate;
}

Matrix4x4 TransformMatrix3D::getTranslate() const 
{
	return matrixTranslate;
}

TransformMatrix3D TransformMatrix3D::getInverse()  {
    update();
    return {matrixAll.inverse().getTransformData()};
}
