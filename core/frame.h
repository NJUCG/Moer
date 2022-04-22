/**
 * @file frame.h
 * @author zcx
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include "core.h"

struct Frame {
    Normal n;
    vec3f s, t;
    
    Frame() { }

    Frame(const Normal &_n) : n(_n) {
        // caculate the s and t
    }

    Frame(const Normal &_n, const vec3f &_s, const vec3f &_t) : n(_n), s(_s), t(_t) { }

    vec3f toWorld() const {
        // todo
    }

    vec3f toLocal() const {
        // todo
    }
};