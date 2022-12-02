/**
 * @file Phase.h
 * @author Chenxi Zhou
 * @brief Abstraction for phase function
 * @version 0.1
 * @date 2022-09-25
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 */

#pragma once

#include <tuple>
#include "CoreLayer/Geometry/Geometry.h"

class PhaseFunction {
public:
    PhaseFunction() = default;

    /**
     * @brief Given the direction of wi/wo (both in world), 
     *  evaluate the phase function
     * 
     * @param wo The direction towards light/next intersection
     * @param wi The direction towards camera/previous intersection
     * @param scatterPoint The position where scatter occurs
     * @return std::tuple<double, double, bool> 
     * phaseValue, phasePdf and whether delta distribution
     */
    virtual std::tuple<double, double, bool> 
    evalPhase(Vec3d wo, Vec3d wi, Point3d scatterPoint) const = 0;

    /**
     * @brief Given the direction of wi (in world),
     *  sample a scatter direction
     * 
     * @param wo 
     * @param scatterPoint 
     * @return std::tuple<Vec3d, double, double, bool>
     * wi, phaseValue, phasePdf and whether delta distribution 
     */
    virtual std::tuple<Vec3d, double, double, bool>
    samplePhase(Vec3d wo, Point3d scatterPoint, Point2d sample) const = 0;

};