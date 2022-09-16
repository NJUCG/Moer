cmake_minimum_required(VERSION 3.12)
project(Zero LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ZERO_DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/ext)
set(ZERO_SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${ZERO_DEPENDENCIES_DIR})
include_directories(${ZERO_SOURCE_DIR})

#default release version
SET(CMAKE_BUILD_TYPE Release)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/Debug/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/Debug/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/Debug/bin)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/Release/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/Release/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/Release/bin)
endif()

## complie for dependencies
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
add_subdirectory(${ZERO_DEPENDENCIES_DIR})

## complie for Zero
file(GLOB_RECURSE ZERO_SOURCES ${ZERO_SOURCES_DIR}/*.cpp)
add_subdirectory(./test)