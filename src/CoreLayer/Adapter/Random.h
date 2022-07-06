/**
 * @file random.h
 * @author Chenxi Zhou
 * @brief The adapter of random number generator
    Currently generate the random number between 0 and 1
 * @version 0.1
 * @date 2022-06-24
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 * 
 */

#pragma once
#include <random>
#include "pcg/pcg_random.hpp"

#include "CoreLayer/Math/Common.h"

class RandomNumberGenerator {
    std::uniform_real_distribution<> dist;
    pcg_extras::seed_seq_from<std::random_device> seedSource;
    pcg32 rng;
public:
    RandomNumberGenerator():dist(0, ONEMINUSEPSILON), rng(seedSource) { }
    
    // default, generate the double in [0, 1)
    double operator()() {
        return dist(rng);
    }

    // generate the int bewteen [begin, end)
    int operator()(int begin, int end) {
        int sample = begin + (end - begin) * dist(rng);
        return std::min(end - 1, sample);
    }

};