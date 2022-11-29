/**
 * @file Random.h
 * @author Chenxi Zhou
 * @brief The class used to generate random numbers
 * @version 0.1
 * @date 2022-06-24
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 * 
 */

#pragma once

#include <random>
#include "pcg_random.hpp"
#include "CoreLayer/Math/Common.h"

/**
 * \brief RandomNumberGenerator, using \link https://www.pcg-random.org/index.html pcg \endlink to generate random number
 * 
 */
class RandomNumberGenerator {

    std::uniform_real_distribution<> dist;
    pcg_extras::seed_seq_from<std::random_device> seedSource;
    pcg32 rng;

public:
    RandomNumberGenerator():dist(0, ONEMINUSEPSILON), rng(seedSource) { }
    // Add this copy cstr to enable auto-generation of other copy cstr
    // for classes having RNG as member variable
    RandomNumberGenerator(const RandomNumberGenerator&): dist(0, ONEMINUSEPSILON), rng(seedSource) { }
    
    /// \brief Generate uniformly distributed random double in [0, 1)
    double operator()() {
        return dist(rng);
    }

    /// \brief Generate uniformly distributed random int in [begin, end)
    int operator()(int begin, int end) {
        int sample = begin + (end - begin) * dist(rng);
        return std::min(end - 1, sample);
    }

};