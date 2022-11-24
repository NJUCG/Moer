cmake_minimum_required(VERSION 3.12)
project(Moer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options("/MP")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    # set unicode as character set
    # add_definitions(-DUNICODE -D_UNICODE)
endif (MSVC)

option(EMBREE_USE_TBB "Enable TBB in embree." OFF)
option(USE_SAMPLED_SPECTRUM "" OFF)

set(MESH_LOADER "Tinyobjloader" CACHE STRING "Select mesh loader")
set_property(CACHE MESH_LOADER PROPERTY STRINGS Assimp Tinyobjloader)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# set target folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/lib)
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_RELEASE_POSTFIX "_r")

## complie for dependencies
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_DOUBLE_PRECISION ON)
add_subdirectory(${ZERO_DEPENDENCIES_DIR})

## complie for Zero
file(GLOB_RECURSE ZERO_SOURCES ${ZERO_SOURCES_DIR}/*.cpp)
add_subdirectory(./test)