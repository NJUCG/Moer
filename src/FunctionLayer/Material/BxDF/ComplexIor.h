/**
 * @file ComplexIor.h
 * @author JunPing.Yuan
 * @brief read eta and k from materialName
 * @version 0.1
 * @date 2022-10-5
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */
#pragma once

#include "CoreLayer/Geometry/Geometry.h"
#include <string>
namespace ComplexIorList {

    bool lookup(const std::string &name, Vec3d &eta, Vec3d &k);

}