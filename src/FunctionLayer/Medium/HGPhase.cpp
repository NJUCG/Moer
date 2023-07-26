#include "HGPhase.h"
#include"CoreLayer/ColorSpace/Color.h"

std::tuple<double, double, bool>
HGPhase::evalPhase(Vec3d wo, Vec3d wi, Point3d scatterPoint) const {
    Vec3d w = wo * wi;
    float cosTheta = (w[0] + w[1] + w[2]) / wo.length() * wi.length();
    float oper = 1 + g * g + 2 * g * cosTheta;
    float phaseValue = 0.25 * INV_PI * (1 - g * g) / (oper * std::sqrt(oper));
    float phasePdf = phaseValue * 0.25 * INV_PI;
    return {phaseValue,phasePdf, false};
}

std::tuple<Vec3d, double, double, bool>
HGPhase::samplePhase(Vec3d wo, Point3d scatterPoint, Point2d sample) const {
    float cosTheta;
    if (std::abs(g) < 1e-3) {
        cosTheta = 1 - 2 * sample[0];
    } 
    else {
        float sqr = (1 - g * g) / (1 + g - 2 * g * sample[0]);
        cosTheta = -(1 + g * g - sqr * sqr) / (2 * g);
    }

    float sinTheta = std::sqrt(std::max((float) 0, 1 - cosTheta * cosTheta));
    float phi = 2 * M_PI * sample[1];
    Vec3d v1, v2;
    
    if (std::abs(wo.x) > std::abs(wo.y)) {
        v1 = Vec3d(-wo.z, 0, wo.x) / std::sqrt(wo.x * wo.x + wo.z * wo.z);
    } else {
        v1 = Vec3d(0, wo.z, -wo.y) / std::sqrt(wo.y * wo.y + wo.z * wo.z);
    }
    v2 = cross(wo, v1);
    Vec3d wi = v1 * sinTheta * std::cos(phi) + v2 * sinTheta * std::sin(phi) + wo * cosTheta;
    float oper = 1 + g * g + 2 * g * cosTheta;
    float phaseValue = 0.25 * INV_PI * (1 - g * g) / (oper * std::sqrt(oper));
    float phasePdf = phaseValue;
    return {wi, phaseValue, phasePdf, false};
}
