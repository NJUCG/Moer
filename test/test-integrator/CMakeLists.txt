cmake_minimum_required(VERSION 3.12)

set(TEST_NAME test-integrator)

AddTestModule(
    ModuleName ${TEST_NAME}
    Src Main.cpp 
    #../../src/Integrator/Integrator.cpp
    #../../src/Integrator/AbstractPathIntegrator.cpp
    #../../src/Integrator/MonteCarloIntegrator.cpp
    #../../src/Integrator/PathIntegrator.cpp
    #../../src/BoundingBox/BoundingBox.cpp
    #../../src/Ray/Ray.cpp
    #../../src/Scene/Scene.cpp
    #../../src/Color/RGB3.cpp
    #../../src/Color/Spectrum.cpp 
    #../../src/Color/XYZ3.cpp
    #../../src/Texture/Texture.cpp
    #../../src/Texture/TextureMapping.cpp
    #../../src/Texture/ImageTexture.cpp
    #../../src/Texture/ProceduralTexture.cpp
    #../../src/Matrix/Matrix.cpp
    #../../src/Light/PointLight.cpp
    #../../src/Light/AreaLight.cpp
    #../../src/Light/DiffuseAreaLight.cpp
    #../../src/Transform/Transform3d.cpp
    #../../src/Entity/Sphere.cpp
    #../../src/Film/Film.cpp
    #../../src/Geometry/CoordConvertor.cpp
    #../../src/Material/MatteMaterial.cpp
    #../../src/Sampler/DirectSampler.cpp
    #../../src/Transform/Transform3d.cpp
    #../../src/Image/Image.cpp
    # ../../src/BxDF/Diffuse.cpp
    # ../../src/BxDF/TestMirrorBxdf.cpp
    # ../../src/BxDF/Microfacet.h
    # ../../src/BxDF/Mircofacet.cpp
    # ../../src/Material/TestMirror.cpp
    # ../../src/Camera/Pinhole.cpp
    # ../../src/Camera/Thinlens.cpp
    ${ZERO_SOURCES}
)


set(EXCNAME ${PROJECT_NAME}_${TEST_NAME})

target_include_directories(${EXCNAME} PUBLIC ${PROJECT_EXTERN_INCLUDE})
target_link_directories(${EXCNAME} PUBLIC ${PROJECT_EXTERN_LIBDIR})

# test for imgui-dx11
# target_link_libraries(app assimp-vc142-mt imgui-dx11 d3d11 d3dcompiler)

# test for opengl3
# target_link_libraries(${EXCNAME} assimp-vc142-mt imgui-opengl3 OpenGL32 glfw3)


if(MSVC AND ((NOT CMAKE_BUILD_TYPE) OR CMAKE_BUILD_TYPE STREQUAL "Debug") )    
    set_property(TARGET ${EXCNAME} APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")
endif()
