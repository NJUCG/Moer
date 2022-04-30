/**
 * @file ray.h
 * @author zcx
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include "point.h"
#include "vector.h"
#include "geometry.h"
#include <iostream>
struct Ray {
    Point3f origin;
    Vec3f dir;
    float time;

    Ray() { }

    Ray(const Point3f &_origin, const Vec3f &_dir) : origin(_origin), dir(_dir), time(0) { }

    Ray(const Point3f &_origin, const Vec3f &_dir, float _time) : origin(_origin), dir(_dir), time(_time) { }

    Point3f operator()(float t) const {
        return origin + t * dir;
    }
};

static inline std::ostream& operator<<(std::ostream &os, const Ray &r) {
    os << "Point: " << r.origin << " Direction: " << r.dir;
    return os; 
}