/**
 * @file Transform3d.h
 * @author orbitchen
 * @brief 3d transformation representation. More like an interface: be inherited means that the inheriting class supports 3d transformation.
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include <memory>
#include "Geometry.h"
#include "Matrix.h"
#include "CoreLayer/Adapter/JsonUtil.h"

/// \ingroup Geometry
/// \brief 3d transformation representation
class Transform3D
{

protected:
    //I move this from private to protected since object needs matrix to apply transform.
    std::shared_ptr<TransformMatrix3D> matrix;
	std::shared_ptr<TransformMatrix3D> matrixInv;
	
public:

	Transform3D();

	Transform3D(std::shared_ptr<Transform3D> _matrix);

    Transform3D(const Json &);

	Point3d getTranslate();
};