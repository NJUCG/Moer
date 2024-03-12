cmake_minimum_required(VERSION 3.19)
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

if(FM_SPEED_DEFAULT)
    add_definitions(-DFM_SPEED_DEFAULT=${FM_SPEED_DEFAULT})
endif (FM_SPEED_DEFAULT)

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

add_subdirectory(ext)
add_subdirectory(src)

option(BUILD_SCENEPREVIEWER "Set this option to enable building of scene_previewer" OFF)
# Moer_ScenePreviewer is a hardware rasterizer designed to offer fast scene previews.
# It is an additional executable that can be built separately.
if(BUILD_SCENEPREVIEWER)
    # Note: Building ScenePreviewer requires additional dependencies compared to Moer. 
    # Please refer to the README in the ScenePreviewer directory for more information.
    # Use 'git submodule update --remote tools/ScenePreviewer' to get the latest version.
    add_subdirectory(tools/ScenePreviewer)
endif()