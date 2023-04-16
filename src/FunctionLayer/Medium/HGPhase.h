/**
 * @file HGPhase.h
 * @author Wanqi Yuan
 * @brief Abstraction for HGphase function
 * @version 0.1
 * @date 2023-04-11
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */
#pragma once

#include "FunctionLayer/Medium/Phase.h"

class HGPhase : public PhaseFunction {
public:
    HGPhase(float g) : g(g) {}

    virtual std::tuple<double, double, bool>
    evalPhase(Vec3d wo, Vec3d wi, Point3d scatterPoint) const override;

    virtual std::tuple<Vec3d, double, double, bool>
    samplePhase(Vec3d wo, Point3d scatterPoint, Point2d sample) const override;

public:
    const float g;
};