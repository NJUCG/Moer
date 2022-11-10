/**
 * @file AreaLight.h
 * @author Zhimin Fan
 * @brief Area light (abstract).
 * @version 0.1
 * @date 2022-05-20
 *
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 *
 */

#pragma once

#include "Light.h"
#include "CoreLayer/Geometry/Transform3d.h"

/// \ingroup Light
/// \brief Base class for all area light source
class AreaLight : public Light
{
protected:
    std::shared_ptr<Transform3D> transform;

public:
    AreaLight(std::shared_ptr<Transform3D> transform3D);
};