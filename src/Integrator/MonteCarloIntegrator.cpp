/**
 * @file MonteCarloIntegrator.h
 * @author Zhimin Fan
 * @brief Integrators
 * @version 0.1
 * @date 2022-05-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "MonteCarloIntegrator.h"

void MonteCarloIntegrator::render(std::shared_ptr<Scene> scene)
{
    // todo
}

double MonteCarloIntegrator::MISWeight(double x, double y)
{
    return x / (x + y);
}

double MonteCarloIntegrator::randFloat()
{
    // todo
    return 0.0;
}