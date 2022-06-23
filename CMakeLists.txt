cmake_minimum_required(VERSION 3.12)
project(Zero LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ZERO_DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/ext)
set(ZERO_SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${ZERO_DEPENDENCIES_DIR})
include_directories(${ZERO_SOURCE_DIR})

## complie for dependencies
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
add_subdirectory(${ZERO_DEPENDENCIES_DIR})

## complie for Zero
file(GLOB_RECURSE ZERO_SOURCES ${ZERO_SOURCES_DIR}/*.cpp)
add_subdirectory(./test)