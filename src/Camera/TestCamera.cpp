/**
 * @file TestCamera.cpp
 * @author Zhimin Fan
 * @brief A fixed camera only for unit test of integrator
 * @version 0.1
 * @date 2022-05-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "TestCamera.h"
#include "../Core/CoreLayer/Matrix.h"

Ray TestCamera::generateRay(Point2d NDC) const
{
    Matrix4x4 mat;
    mat = mat.lookAt(Point3d(0.0, 1.0, 2.0), Vec3d(0.0, -1.0, -2.0), Vec3d(0.0, 1.0, 0.0));
    Ray ray(Point3d(0.0, 1.0, 2.0), normalize(mat.inverse() * Vec3d(NDC.x, NDC.y, 1.0)));
    return ray;
}

Ray TestCamera::generateRay(Point2d NDC, Point2d sample) const {
    // do thing
}