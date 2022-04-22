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

struct Normal : public TVector3<float> {
    
    Normal() { }

    Normal (const TVector3<float> &v) : TVector3<float>(normalize(v)) { }

    Normal (float _x, float _y, float _z) : Normal(TVector3<float> {_x, _y, _z})  { }

    
}; 