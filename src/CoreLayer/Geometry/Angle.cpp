#include <cmath>
#include "Angle.h"
#include "CoreLayer/Math/Common.h"

// implemention of class Angle

Angle::Angle(){
	// empty
}

Angle::Angle(double v, EAngleType type)
{
	if (type == EAngleType::ANGLE_DEG)
	{
		deg = v;
		rad = v / 180 * M_PI;
	}
	else
	{
		rad = v;
		deg = v * 180 * INV_PI;
	}
}

double Angle::getDeg() const { return deg; }

double Angle::getRad() const { return rad; }

double cos(const Angle& a) {return cos(a.getRad());}

double sin(const Angle& a) {return sin(a.getRad());}

double tan(const Angle& a) {return tan(a.getRad());}

// implemention of class Polar2d

Polar2d::Polar2d(const Point2d& p){
	Vec2d vec=Vec2d(p.x,p.y);
	distance=vec.length();
	if(vec.x==0.0 && vec.y > 0) azimuth=Angle(90.0,Angle::EAngleType::ANGLE_DEG);
	else if(vec.x==0 && vec.y < 0) azimuth=Angle(-90.0,Angle::EAngleType::ANGLE_DEG);
	else if(vec.x==0.0 && vec.y==0.0) azimuth=Angle(0.0);
	else azimuth=Angle(acos(distance/vec.x),Angle::EAngleType::ANGLE_RAD);
}

Polar2d::Polar2d(const Vec2d& p){
	const Vec2d& vec=p;
	distance=vec.length();
	if(vec.x==0.0 && vec.y > 0) azimuth=Angle(90.0,Angle::EAngleType::ANGLE_DEG);
	else if(vec.x==0 && vec.y < 0) azimuth=Angle(-90.0,Angle::EAngleType::ANGLE_DEG);
	else if(vec.x==0.0 && vec.y==0.0) azimuth=Angle(0.0);
	else azimuth=Angle(acos(distance/vec.x),Angle::EAngleType::ANGLE_RAD);
}

Polar2d::Polar2d(double _distance, const Angle& angle)
	:distance(_distance)
	,azimuth(angle)
{
	// empty
}

Vec2d Polar2d::toVec2d(){
	Vec2d ret;
	ret.x=distance*cos(azimuth);
	ret.y=distance*sin(azimuth);
	return ret;
}

Point2d Polar2d::toPoint2d(){
	Point2d ret;
	ret.x=distance*cos(azimuth);
	ret.y=distance*sin(azimuth);
	return ret;
}

// implemention of class Polar3d

Polar3d::Polar3d(const Point3d& p){
	Vec3d vec=Vec3d(p.x,p.y,p.z);
	distance=vec.length();

	if (vec.z==0.0) elevation=Angle(0.0);
	else elevation=Angle(distance/vec.z,Angle::EAngleType::ANGLE_RAD);

	if(vec.x==0.0 && vec.y > 0) azimuth=Angle(90.0,Angle::EAngleType::ANGLE_DEG);
	else if(vec.x==0.0 && vec.y < 0) azimuth=Angle(-90.0,Angle::EAngleType::ANGLE_DEG);
	else if(vec.x==0.0 && vec.y==0.0) azimuth=Angle(0.0);
	else azimuth=Angle(distance*cos(elevation)/vec.x,Angle::EAngleType::ANGLE_RAD);
}

Polar3d::Polar3d(const Vec3d& p){
	const Vec3d& vec=p;
	distance=vec.length();

	if (vec.z==0.0) elevation=Angle(0.0);
	else elevation=Angle(distance/vec.z,Angle::EAngleType::ANGLE_RAD);

	if(vec.x==0.0 && vec.y > 0) azimuth=Angle(90.0,Angle::EAngleType::ANGLE_DEG);
	else if(vec.x==0.0 && vec.y < 0) azimuth=Angle(-90.0,Angle::EAngleType::ANGLE_DEG);
	else if(vec.x==0.0 && vec.y==0.0) azimuth=Angle(0.0);
	else azimuth=Angle(distance*cos(elevation)/vec.x,Angle::EAngleType::ANGLE_RAD);
}

Polar3d::Polar3d(double _distance,const Angle& _azimuth, const Angle& _elevation)
	:distance(_distance)
	,azimuth(_azimuth)
	,elevation(_elevation)
{
	// empty
}

Vec3d Polar3d::toVec3d(){
	Vec3d ret;
	ret.x=distance*cos(elevation)*cos(azimuth);
	ret.y=distance*cos(elevation)*sin(azimuth);
	ret.z=distance*sin(elevation);
	return ret;
}

Point3d Polar3d::toPoint3d(){
	Point3d ret;
	ret.x=distance*cos(elevation)*cos(azimuth);
	ret.y=distance*cos(elevation)*sin(azimuth);
	ret.z=distance*sin(elevation);
	return ret;
}