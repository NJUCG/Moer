/**
 * @file Curve.h
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
#include "FunctionLayer/Acceleration/Embree.h"

class CurveSegment;

class Curve : public  Entity{
public:
    Curve(const Json &json);

    virtual std::optional<Intersection> intersect(const Ray &r) const override;

    virtual double area() const override;

    virtual Intersection sample(const Point2d &positionSample) const override;

    virtual BoundingBox3f WorldBound() const override;



protected:

    std::vector<int> _curveEnds;
    std::vector<Vec4d> _nodeData;
    std::vector<Vec3d> _nodeColor;
    std::vector<Vec3d> _nodeNormals;

    std::vector<int> _indices;

   // RTCScene rtcScene;
   // RTCGeometry rtcGeometry;
    std:: unique_ptr<EmbreeAccel> curveScene;


    std::vector<std::shared_ptr<Entity>> curveSegments;
    RTCScene scene;
    RTCGeometry geometry;
    double _curveThickness;
    bool _overrideThickness;

    size_t _curveCount;

    BoundingBox3f bb;
    virtual void apply() override;
};