cmake_minimum_required(VERSION 3.12)
## compile for catch2
add_subdirectory(./catch2)

set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_DOUBLE_PRECISION on)
set(ASSIMP_WARNINGS_AS_ERRORS off)
add_subdirectory(./assimp)
#add_subdirectory(./embree3)