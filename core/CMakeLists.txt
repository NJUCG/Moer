cmake_minimum_required(VERSION 3.12)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/)

add_executable(coreTest ./core_test.cpp)
target_include_directories(coreTest PUBLIC "${PROJECT_SOURCE_DIR}/core")
