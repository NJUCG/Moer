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

#include "Transform3d.h"
#include "CoreLayer/Math/Common.h"
#include "FastMath.h"

static Vec3d randomOrtho(const Vec3d &a)
{
    Vec3d res;
    if (fm::abs(a.x) > fm::abs(a.y))
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

Transform3D::Transform3D(const Json & json) {
    if(json.is_null()){
        matrix = std::make_shared<TransformMatrix3D>();
    }
    else if(json.is_array()){
        double matrixData[16];
        for(int i=0;i<16;i++)
            json.at(i).get_to(matrixData[i]);
        matrix = std::make_shared<TransformMatrix3D>(matrixData);
    }
    else{
        matrix = std::make_shared<TransformMatrix3D>();
        //todo modify this
        Point3d pos= getOptional(json,"position",Point3d(0.0));
        this->matrix->setTranslate(pos.x,pos.y,pos.z);
        Vec3d scale = getOptional(json,"scale",Vec3d(1,1,1));
        this->matrix->setScale(scale.x,scale.y,scale.z);
        Vec3d rotate = getOptional(json,"rotation",Vec3d(0,0,0));
        this->matrix->setRotateEuler(Angle(rotate.x,Angle::EAngleType::ANGLE_DEG),
                                     Angle(rotate.y,Angle::EAngleType::ANGLE_DEG),
                                     Angle(rotate.z,Angle::EAngleType::ANGLE_DEG),
                                     EulerType::EULER_YZX);
    }
}
