/**
 * @file Path.cpp
 * @author Zhimin Fan
 * @brief Path Integrator
 * @version 0.1
 * @date 2022-05-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Path.h"

Spectrum PathIntegrator::Li(const Ray &ray0, std::shared_ptr<Scene> scene)
{
    Spectrum L(0.0);
    Spectrum T(0.0); // throughput

    Ray ray = ray0;
    double pdfB = 0.0; // pdf of last BSDF sample
    int nBounce = 0;

    while (true)
    {
        Intersection its = scene->intersect(ray);
        Vec3f wo = -ray.direction;
        Vec3f n; // todo: get shading normal

        // hit emitter or env
        {
            Spectrum Le(0.0f);
            float pdfL = 1.0f;
            if (its.object)
            {
                // todo: get environment Le and pdfL
            }
            else if (/* hit object is emitter */ 0)
            {
                // todo: get emitted Le and pdfL
            }
            float mis = MISWeight(pdfB, pdfL);
            Spectrum transmittance(1.0f); // todo: transmittance eval
            L += T * Le / pdfL * mis;
        }

        // terminate path
        double q = std::min(0.95, T.sum());
        if (randFloat() < q && its.object)
        {
            T /= q;
        }
        else
        {
            break;
        }
        nBounce++;

        std::shared_ptr<BxDF> bxdf = its.material->getBxDF(its);

        // sample light
        {
            // todo: sample all lights and fill these vars below
            float pdfL; // pdf with respect to solid angle, to our intersection
            Vec3f wi;
            Spectrum Li;
            Point3f posL;
            // todo: media
            Spectrum bsdf = bxdf->f(-ray.direction, wi);
            float pdfB = bxdf->pdf(wo, wi);
            float mis = MISWeight(pdfL, pdfB);
            float cosine = dot(n, wi);
            Spectrum transmittance(1.0f); // todo: visibility test + transmittance eval
            L += T * Li * bsdf * cosine * transmittance / pdfL * mis;
        }

        // sample scatter
        {
            // todo: media
            BxDFSampleResult bsdfSample = bxdf->sample(wo);
            pdfB = bsdfSample.pdf;
            Vec3f wi = bsdfSample.directionIn;
            T *= bsdfSample.s * dot(wi, n) * pdfB;
            ray = Ray(its.position, wi); // todo: set time
        }
    }

    return L;
}
