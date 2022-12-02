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

option(${CMAKE_PROJECT_NAME}_EMBREE_USE_TBB "Enable TBB in embree." OFF)
option(${CMAKE_PROJECT_NAME}_USE_SAMPLED_SPECTRUM "" OFF)

set(${CMAKE_PROJECT_NAME}_SPECTRUM_SAMPLES 60 CACHE STRING "The number of spectrum samples")

set(${CMAKE_PROJECT_NAME}_MESH_LOADER "Tinyobjloader" CACHE STRING "Select mesh loader")
set_property(CACHE ${CMAKE_PROJECT_NAME}_MESH_LOADER PROPERTY STRINGS Assimp Tinyobjloader)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# set target folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/lib)
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_RELEASE_POSTFIX "_r")

add_subdirectory(ext)
add_subdirectory(src)