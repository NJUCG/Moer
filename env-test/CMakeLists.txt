cmake_minimum_required(VERSION 3.12)
# test for dx11
# add_executable(app ./main.cpp ./imgui_dx11_test.cpp)

# test for opengl3
add_executable(app ./main.cpp ./imgui_opengl3_test.cpp)

target_include_directories(app PUBLIC ${PROJECT_EXTERN_INCLUDE})
target_link_directories(app PUBLIC ${PROJECT_EXTERN_LIBDIR})
# test for imgui-dx11
# target_link_libraries(app assimp-vc142-mt imgui-dx11 d3d11 d3dcompiler)

# test for opengl3
target_link_libraries(app assimp-vc142-mt imgui-opengl3 OpenGL32 gsl glfw3)