/**
 * @file Angle.h
 * @author orbitchen
 * @brief Angle representation and 3d polar coordinate system.
 * @version 0.2
 * @date 2022-11-1
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma once

#include "Geometry.h"

/**
  * @brief Angle type. Providing convenient angle transformation between deg and rad.
  */
class Angle
{
	double deg;
	double rad;

public:
	enum class EAngleType
	{
		ANGLE_DEG,
		ANGLE_RAD
	};

    Angle();

	/**
	 * @brief init an angle.
	 * @param v value of angle, deg or rad.
	 * @param type the type of v.
	 */
	Angle(double v, EAngleType type=EAngleType::ANGLE_RAD);

	double getDeg() const;

	double getRad() const;
};

double cos(const Angle& a);

double sin(const Angle& a);

double tan(const Angle& a);

/// @brief 2d polar coordinate.
class Polar2d{

private:

    /// @brief distance to (0,0)
    double distance;

    /// @brief azimuth angle. rotation: +x +y -x -y
    Angle azimuth;

public:

    Polar2d(const Point2d& p);

    Polar2d(const Vec2d& p);

    Polar2d(double _distance, const Angle& angle);

    Vec2d toVec2d();

    Point2d toPoint2d();

};

/// @brief 3d polar coordinate.
class Polar3d{

private:

    /// @brief distance to (0,0)
    double distance;

    /// @brief azimuth angle. rotation: +x +y -x -y
    Angle azimuth;

    /// @brief elevation angle. 
    Angle elevation;

public:

    Polar3d(const Point3d& p);

    Polar3d(const Vec3d& p);

    Polar3d(double _distance,const Angle& _azimuth, const Angle& _elevation);

    Vec3d toVec3d();

    Point3d toPoint3d();

};