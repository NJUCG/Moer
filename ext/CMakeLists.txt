cmake_minimum_required(VERSION 3.12)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/lib)
## compile for catch2
add_subdirectory(./catch2)
add_subdirectory(./assimp)
add_subdirectory(./glfw)
add_subdirectory(./imgui)