/**
 * @file AreaLight.cpp
 * @author Zhimin Fan
 * @brief Area light (abstract)
 * @version 0.1
 * @date 2022-05-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "AreaLight.h"

AreaLight::AreaLight(std::shared_ptr<Transform3D> transform3D) : transform3D(transform3D)
{
}