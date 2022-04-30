/**
 * @file shape.h
 * @author zcx
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "ray.h"
#include "normal.h"
#include "geometry.h"
#include <memory>
struct Intersection {
    // pointer to the underlying shape
    std::shared_ptr<Shape> shapePtr;

    // position of the hit point
    Point3f p;
    
    // normal at the hit point
    Normal3f n;
    
    // the length from ray origin to the hit point  
    float t;

    // the uv coordinate
    Point2f uv;

    // the direction of the incident ray
    Vec3f wi;
};

class Shape {

public:
    virtual bool intersect(const Ray& ray, Intersection &its, float tmin, float tmax) const = 0;

    virtual bool intersect(const Ray& ray, float tmin, float tmax) const = 0;

};