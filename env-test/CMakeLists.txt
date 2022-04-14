cmake_minimum_required(VERSION 3.12)

add_executable(app ./main.cpp ./imgui_dx11_test.cpp)
target_include_directories(app PUBLIC ${PROJECT_EXTERN_INCLUDE})
target_link_directories(app PUBLIC ${PROJECT_EXTERN_LIBDIR})
target_link_libraries(app assimp-vc142-mt imgui d3d11 d3dcompiler)