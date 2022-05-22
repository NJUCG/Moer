cmake_minimum_required(VERSION 3.12)

set(TEST_NAME test-env)

# test for imgui-dx11
# AddTestModule(
#     ModuleName ${TEST_NAME}
#     Src main.cpp imgui_dx11_test.cpp
# )

# test for opengl3
AddTestModule(
    ModuleName ${TEST_NAME}
    Src main.cpp imgui_opengl3_test.cpp
)

set(EXCNAME ${PROJECT_NAME}_${TEST_NAME})

target_include_directories(${EXCNAME} PUBLIC ${PROJECT_EXTERN_INCLUDE})
target_link_directories(${EXCNAME} PUBLIC ${PROJECT_EXTERN_LIBDIR})

# test for imgui-dx11
# target_link_libraries(app assimp-vc142-mt imgui-dx11 d3d11 d3dcompiler)

# test for opengl3
target_link_libraries(${EXCNAME} assimp-vc142-mt imgui-opengl3 OpenGL32 glfw3)


if(MSVC AND ((NOT CMAKE_BUILD_TYPE) OR CMAKE_BUILD_TYPE STREQUAL "Debug") )    
    set_property(TARGET ${EXCNAME} APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")
endif()

add_custom_command(
    TARGET ${EXCNAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} 
        -E copy_if_different
            "${PROJECT_EXTERN_LIBDIR}/assimp-vc142-mt.dll"
            $<TARGET_FILE_DIR:${EXCNAME}>
) 