set(TARGET_NAME Moer)

file(GLOB_RECURSE ALL_INCLUDE CONFIGURE_DEPENDS "CoreLayer/*.h" "FunctionLayer/*.h" "ResourceLayer/*.h")
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS  "CoreLayer/*.cpp" "FunctionLayer/*.cpp" "ResourceLayer/*.cpp")



source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_INCLUDE} ${ALL_SOURCES})

add_executable(${TARGET_NAME} ${ALL_INCLUDE} ${ALL_SOURCES} main.cpp FunctionLayer/Material/BumpMapMateiral.cpp)
add_executable(bsdfTest ${ALL_INCLUDE} ${ALL_SOURCES}  brdfTest.cpp FunctionLayer/Material/NormalMap.cpp FunctionLayer/Material/NormalMap.h FunctionLayer/Material/BxDF/BumpMap.cpp FunctionLayer/Material/BxDF/BumpMap.h FunctionLayer/Material/BumpMapMaterial.h)
target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(bsdfTest PUBLIC ${PROJECT_SOURCE_DIR}/src)
add_dependencies(${TARGET_NAME} ext-copy)
add_dependencies(bsdfTest ext-copy)
target_link_libraries(${TARGET_NAME} PUBLIC Moer-ext)
target_link_libraries(bsdfTest PUBLIC Moer-ext)

find_package(Threads)
target_link_libraries(${TARGET_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(bsdfTest PUBLIC ${CMAKE_THREAD_LIBS_INIT})

if (MESH_LOADER STREQUAL "Assimp")
    target_compile_definitions(${TARGET_NAME} PRIVATE MESH_LOADER_ASSIMP)
elseif (MESH_LOADER STREQUAL "Tinyobjloader")
    target_compile_definitions(${TARGET_NAME} PRIVATE MESH_LOADER_TINYOBJ)
    target_compile_definitions(bsdfTest PRIVATE MESH_LOADER_TINYOBJ)
endif()

if (USE_SAMPLED_SPECTRUM)
    target_compile_definitions(${TARGET_NAME} PRIVATE USING_SAMPLED_SPECTRUM)
    target_compile_definitions(bsdfTest PRIVATE USING_SAMPLED_SPECTRUM)
endif()

target_compile_definitions(${TARGET_NAME} PRIVATE CMAKE_DEF_SPECTRUM_SAMPLES=60)
target_compile_definitions(bsdfTest PRIVATE CMAKE_DEF_SPECTRUM_SAMPLES=60)

set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(bsdfTest PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "_r")
set_target_properties(bsdfTest PROPERTIES RELEASE_POSTFIX "_r")
