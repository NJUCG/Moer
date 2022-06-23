#include "DirectSampler.h"

double DirectSampler::sample() const
{
    return rand() * 1.0 / RAND_MAX;
}

std::vector<double> DirectSampler::sample(int num) const
{
    return {};
}