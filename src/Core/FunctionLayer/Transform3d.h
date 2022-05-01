/**
 * @file Transform3d.h
 * @author orbitchen
 * @brief 3d transformation representation. More like an interface: be inherited means that the inheriting class supports 3d transformation.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../CoreLayer/Geometry.h"
#include "../CoreLayer/Matrix.h"

#include <memory>

class Transform3D
{

	std::shared_ptr<TransformMatrix3D> matrix;

	bool isDone = false;

protected:
	// apply matrix to this object. Cache should be managed locally.
	void apply() = 0;

public:
	Transform3D();

	Transform3D(std::shared_ptr<Transform3D> _matrix);

	void setTranslate(double x, double y, double z);

	void setScale(double x, double y, double z);
	void setScale(double ratio);

	void setRotateEuler(Angle x, Angle y, Angle z, EulerType type = EulerType::EULER_XYZ);

	void setRotateQuaternion(double w, double x, double y, double z);

	// @brief Rotate by axis. Counterclockwise rotate.
	void setRotateAxis(Angle angle, Vec3f axis);

	// @brief inform this object that transform setting is DONE and 'you' can apply all transformation without redundant calculation. apply() should be called within.
	void done();
};