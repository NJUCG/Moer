/**
 * @file IsotropicPhase.h
 * @author Chenxi Zhou
 * @brief Isotropic phase-function
 * @version 0.1
 * @date 2022-09-25
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#pragma once

#include "Phase.h"

class IsotropicPhase : public PhaseFunction {
public:
    IsotropicPhase() = default;

    virtual std::tuple<double, double, bool> 
    evalPhase(Vec3d wo, Vec3d wi, Point3d scatterPoint) const override;

    virtual std::tuple<Vec3d, double, double, bool>
    samplePhase(Vec3d wo, Point3d scatterPoint, Point2d sample) const override;

};