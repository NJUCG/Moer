/**
 * @file Transform3d.cpp
 * @author Zhimin Fan
 * @brief 3d transformation representation impl.
 * @version 0.1
 * @date 2022-05-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "../Core/FunctionLayer/Transform3d.h"

Transform3D::Transform3D() : matrix(std::make_shared<TransformMatrix3D>())
{
}

Point3d Transform3D::getTranslate()
{
    Point3d p(0.0);
    return *matrix * p;
}

void Transform3D::setTranslate(double x, double y, double z)
{
    matrix->setTranslate(x, y, z);
}
