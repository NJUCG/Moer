/**
 * @file core_test.cpp
 * @author zcx
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "core.h"
#include <iostream>

int main() {
    Ray r {point3f{.0f, .0f, .0f}, vec3f {1.f, 2.f, 3.f}};
    std::cout << r << std::endl;

    std::cout << r(2.5f) << std::endl;
}