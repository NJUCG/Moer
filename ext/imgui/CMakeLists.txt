cmake_minimum_required(VERSION 3.12)

add_library(imgui
    ./imgui_demo.cpp
    ./imgui_draw.cpp
    ./imgui_impl_glfw.cpp
    ./imgui_impl_opengl3.cpp
    ./imgui_tables.cpp
    ./imgui_widgets.cpp
    ./imgui.cpp
)

target_link_libraries (imgui
    glfw
)