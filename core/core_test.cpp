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
    vec3f v1 {1, 1, 1};
    std::cout << v1.length() << std::endl;
    std::cout << normalize(v1) << std::endl;
}