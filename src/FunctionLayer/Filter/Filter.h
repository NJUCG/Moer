/**
 * @file Filter.h
 * @author Zhimin Fan
 * @brief Linear filters
 * @version 0.1
 * @date 2022-05-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "CoreLayer/Geometry/Geometry.h"

// ! Note that we cannot use any non-linear "filter" here, such as bilteral and etc

// @brief: Base class for linear filter used for film
class Filter
{
public:
    virtual float eval(const Point2d &pos) = 0;

    // @brief: also known as Kernel Size
    virtual Point2d support() = 0;
};

