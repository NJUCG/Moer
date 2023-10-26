/**
 * @file ZeroTwoSequence.h
 * @author fengzesen (ja50n zs_feng@qq.com)
 * @brief PixelSampler using (0, 2)-sequence.
 *
 * @version 0.1
 * @date 2023-08-10
 * @copyright NJUMeta (c) 2023
 * www.njumeta.com
 */
#pragma once
#include "FunctionLayer/Sampler/ZeroTwoSequence.h"
#include "CoreLayer/Math/LowDiscrepancy.h"

void ZeroTwoSampler::generateSamples1D(std::vector<double> &samples) {
    fillVDCorput1D(1, samplesPerPixel, &samples[0], rng);
}
void ZeroTwoSampler::generateSamples2D(std::vector<Point2d> &samples) {
    fillSobol2D(1, samplesPerPixel, &samples[0], rng);
}