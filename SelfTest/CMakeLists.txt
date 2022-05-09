cmake_minimum_required(VERSION 3.12)
project(SelfTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/)

add_executable(
    SelfTest 
    Main.cpp
    ../src/Integrator/Path.cpp
    ../src/Integrator/MonteCarlo.cpp
    ../src/BoundingBox/BoundingBox.cpp
    ../src/Ray/Ray.cpp
    ../src/Scene/Scene.cpp
    ../src/Color/RGB3.cpp
    ../src/Color/Spectrum.cpp 
    ../src/Color/XYZ3.cpp
)

target_include_directories(SelfTest PUBLIC ${PROJECT_SOURCE_DIR}/../src)
target_include_directories(SelfTest PUBLIC ${PROJECT_SOURCE_DIR}/../ext)
target_link_directories(SelfTest PUBLIC ${PROJECT_EXTERN_LIBDIR})

if(MSVC AND ((NOT CMAKE_BUILD_TYPE) OR CMAKE_BUILD_TYPE STREQUAL "Debug") )    
    set_property(TARGET SelfTest APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")
endif()