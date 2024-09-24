#include "IsotropicPhase.h"
#include "CoreLayer/Math/Warp.h"


std::tuple<Spectrum, double, bool>
IsotropicPhase::evalPhase(Vec3d wo, Vec3d wi, Point3d scatterPoint) const 
{
    return {0.25 * INV_PI, 0.25 * INV_PI, false};
}

std::tuple<Vec3d, Spectrum, double, bool>
IsotropicPhase::samplePhase(Vec3d wo, Point3d scatterPoint, Point2d sample) const 
{
    return {SquareToUniformSphere(sample), 0.25 * INV_PI, 0.25 * INV_PI, false};
}