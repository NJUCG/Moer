cmake_minimum_required(VERSION 3.12)

project(Zero LANGUAGES CXX)

set(PROJECT_EXTERN_INCLUDE "${PROJECT_SOURCE_DIR}/ext")
set(PROJECT_EXTERN_LIBDIR "${PROJECT_SOURCE_DIR}/extlib")
message(STATUS ${PROJECT_EXTERN_INCLUDE})
add_subdirectory(env-test ./out)
# add_subdirectory(core)
