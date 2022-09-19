/**
 * @file Transform3d.cpp
 * @author Zhimin Fan
 * @brief 3d transformation representation impl.
 * @version 0.1
 * @date 2022-05-20
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */
#pragma once

#include "Transform3d.h"
#include "CoreLayer/Math/Common.h"

static Vec3d randomOrtho(const Vec3d &a)
{
    Vec3d res;
    if (std::abs(a.x) > std::abs(a.y))
        res = Vec3d(0.0f, 1.0f, 0.0f);
    else
        res = Vec3d(1.0f, 0.0f, 0.0f);
    return normalize(cross(a,res));
}

static void gramSchmidt(Vec3d &a, Vec3d &b, Vec3d &c)
{
    a= normalize(a);
    b -= a*dot(a,b);
    if ( b.length2() < 1e-5)
        b = randomOrtho(a);
    else
        b= normalize(b);

    c -= a*dot(a,c);
    c -= b*dot(b,c);
    if ( c.length2() < 1e-5)
        c =cross(a,b);
    else
        c= normalize(c);
}


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

Transform3D::Transform3D(const Json json) {
    //todo modify this
    matrix = std::make_shared<TransformMatrix3D>() ;
    Vec3d x(1.0, 0.0, 0.0);
    Vec3d y(0.0, 1.0, 0.0);
    Vec3d z(0.0, 0.0, 1.0);

    Point3d pos= getOptional(json,"position",Point3d(0.0));
    this->matrix->setTranslate(pos.x,pos.y,pos.z);

    bool explicitX = false, explicitY = false, explicitZ = false;


    explicitX = containsAndGet(json,"x_axis", x) || explicitX;
    explicitY = containsAndGet(json,"y_axis", y) || explicitY;
    explicitZ = containsAndGet(json,"z_axis", z) || explicitZ;

    int id =
            ( explicitZ ? 4 : 0 ) +
            ( explicitY ? 2 : 0 ) +
            ( explicitX ? 1 : 0 );
    switch ( id ) {
        case 0:
            gramSchmidt(z, y, x);
            break;
        case 1:
            gramSchmidt(x, z, y);
            break;
        case 2:
            gramSchmidt(y, z, x);
            break;
        case 3:
            gramSchmidt(y, x, z);
            break;
        case 4:
            gramSchmidt(z, y, x);
            break;
        case 5:
            gramSchmidt(z, x, y);
            break;
        case 6:
            gramSchmidt(z, y, x);
            break;
        case 7:
            gramSchmidt(z, y, x);
            break;
    }

    if ( dot(cross(x, y), z) < 0.0f ) {
        if ( ! explicitX )
            x = - x;
        else if ( ! explicitY )
            y = - y;
        else
            z = - z;
    }

    Vec3d scale = getOptional(json,"scale",Vec3d(1,1,1));
        scale = json["scale"];
        x *= scale.x;
        y *= scale.y;
        z *= scale.z;
    this->matrix->setScale(scale.x,scale.y,scale.z);
    scale /= (180/M_PI);
    Vec3d rotate = getOptional(json,"rotation",Vec3d(0,0,0));
    this->matrix->setRotateEuler(Angle(rotate.x,Angle::AngleType::ANGLE_DEG),
                                 Angle(rotate.y,Angle::AngleType::ANGLE_DEG),
                                 Angle(rotate.z,Angle::AngleType::ANGLE_DEG),
                                 EulerType::EULER_YZX);
}
