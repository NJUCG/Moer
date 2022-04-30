/**
 * @file normal.h
 * @author zcx
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include "vector.h"

struct Normal3f : public TVector3<float> {
    
    Normal3f() { }

    Normal3f (const TVector3<float> &v) : TVector3<float>(normalize(v)) { }

    Normal3f (float _x, float _y, float _z) : Normal3f(TVector3<float> {_x, _y, _z})  { }

    
}; 