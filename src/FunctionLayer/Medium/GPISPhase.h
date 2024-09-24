#pragma once

#include "Phase.h"
#include "CoreLayer/Adapter/JsonUtil.h"
#include "FunctionLayer/Material/Material.h"
#include "FunctionLayer/Material/BxDF/BxDF.h"

// MARK(Cchen77):
// we treat GPIS as a special medium and its phase function can be any bxdf.
// our implmentation now is a little bit tricky now,we just utilize the material factory to produce material which then produce bxdf for evaling&sampling
// since we have not suitable mapping system for meidum,do not use materials with texture.
class GPISPhase : public PhaseFunction {
public:
    GPISPhase() = default;
    GPISPhase(const Json &json);

    virtual std::tuple<Spectrum, double, bool> evalPhase(Vec3d wo, Vec3d wi, Point3d scatterPoint) const override;

    virtual std::tuple<Vec3d, Spectrum, double, bool>
    samplePhase(Vec3d wo, Point3d scatterPoint, Point2d sample) const override;

protected:

    std::shared_ptr<BxDF> innerBxDF;
};