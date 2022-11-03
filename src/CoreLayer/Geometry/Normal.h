/**
 * @file Normal.h
 * @author Chenxi Zhou
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 * 
 */

#pragma once

#include "Vector.h"

/// \ingroup Geometry
/// \brief Normal
struct Normal3d : public TVector3<double> {
    
    Normal3d() { }

    Normal3d (const TVector3<double> &v) : TVector3<double>(normalize(v)) { }

    Normal3d (double _x, double _y, double _z) : Normal3d(TVector3<double> {_x, _y, _z})  { }
    
}; 