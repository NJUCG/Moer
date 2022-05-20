cmake_minimum_required(VERSION 3.12)
project(SelfTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/target/)

add_executable(
    SelfTest 
    Main.cpp
    ../src/Integrator/AbstractPathIntegrator.cpp
    ../src/Integrator/PathIntegrator.cpp
    ../src/Integrator/MonteCarloIntegrator.cpp
    ../src/BoundingBox/BoundingBox.cpp
    ../src/Ray/Ray.cpp
    ../src/Scene/Scene.cpp
    ../src/Color/RGB3.cpp
    ../src/Color/Spectrum.cpp 
    ../src/Color/XYZ3.cpp
    ../src/Texture/Texture.cpp
    ../src/Texture/TextureMapping.cpp
    ../src/Texture/ImageTexture.cpp
    ../src/Texture/ProceduralTexture.cpp
    ../src/Matrix/Matrix.cpp
    ../src/Light/PointLight.cpp
    ../src/Light/AreaLight.cpp
    ../src/Light/DiffuseAreaLight.cpp
    ../src/Transform/Transform3d.cpp
)

target_include_directories(SelfTest PUBLIC ${PROJECT_SOURCE_DIR}/../src)
target_include_directories(SelfTest PUBLIC ${PROJECT_SOURCE_DIR}/../ext)
target_link_directories(SelfTest PUBLIC ${PROJECT_EXTERN_LIBDIR})

if(MSVC AND ((NOT CMAKE_BUILD_TYPE) OR CMAKE_BUILD_TYPE STREQUAL "Debug") )    
    set_property(TARGET SelfTest APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")
endif()

set_property(TARGET SelfTest PROPERTY CXX_STANDARD 17)
set_property(TARGET SelfTest PROPERTY CXX_STANDARD_REQUIRED ON)