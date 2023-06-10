/**
 * @file Cube.h
 * @author JunPing Yuan
 * @brief Curve Shape
 * @version
 * @date 2022-09-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "Entity.h"
class Curve : public  Entity{
public:
    Curve(const Json &json);

    virtual std::optional<Intersection> intersect(const Ray &r) const override;

    virtual double area() const override;

    virtual Intersection sample(const Point2d &positionSample) const override;

    virtual BoundingBox3f WorldBound() const override;

    RTCGeometry toEmbreeGeometry(RTCDevice device) const override;

    std::optional < Intersection > getIntersectionFromRayHit(const UserRayHit1 & rayhit) const override;

protected:

    std::vector<int> _curveEnds;
    std::vector<double> _nodeData;
    std::vector<Vec3d> _nodeColor;
    std::vector<Vec3d> _nodeNormals;

    std::vector<int> _indices;

   // RTCScene rtcScene;
   // RTCGeometry rtcGeometry;

    double _subSample;
    double _curveThickness;
    bool _overrideThickness;

    int _curveCount;

    BoundingBox3f bb;
    virtual void apply() override;
    Point3d getPoint(int index) const ;
};