#pragma once

#include "FunctionLayer/Material/BxDF/BxDF.h"
#include "vmm.h"

namespace PathGuiding {

class GuidedBxDF {
public:

    using VMFMixture = vmm::VMFMixture;

    constexpr static double bxdfFraction = 0.5;

    VMFMixture model;
    BxDF * bxdf{};

    [[nodiscard]]
    BxDFSampleResult sample(const Intersection & its, const Vec3d & out, const Point2d & rn, bool adjoint) const {
        BxDFSampleResult result;

        if (rn.x < bxdfFraction) {
            // sample the BxDF
            Point2d reused(rn.x / bxdfFraction, rn.y);
            result = bxdf->sample(out, reused, adjoint);

            // one-sample MIS
            Vec3d direction = its.toWorld(result.directionIn);
            double pdfModel = model.pdf(direction);
            result.pdf = lerp(pdfModel, result.pdf, bxdfFraction);
        } else {
            // sample the model
            Vec2d reused((rn.x - bxdfFraction) / (1 - bxdfFraction), rn.y);
            Vec3d direction = model.sample(reused);
            double pdfModel = model.pdf(direction);
            result.directionIn = its.toLocal(direction);
            result.s = bxdf->f(out, result.directionIn, adjoint);

            // one-sample MIS
            result.pdf = lerp(pdfModel, bxdf->pdf(out, result.directionIn), bxdfFraction);
            // TODO result.bxdfSampleType
        }

        return result;
    }

    [[nodiscard]]
    double pdf(const Intersection & its, const Vec3d & out, const Vec3d & in) const {
        Vec3d direction = its.toWorld(in);
        double pdfModel = model.pdf(direction);
        double pdfBxDF = bxdf->pdf(out, in);
        return lerp(pdfModel, pdfBxDF, bxdfFraction);
    }
};

}
