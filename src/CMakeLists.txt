set(TARGET_NAME Moer)

set(CORE_LAYER_DIR ${PROJECT_SOURCE_DIR}/src/CoreLayer)
set(FUNCTION_LAYER_DIR ${PROJECT_SOURCE_DIR}/src/FunctionLayer)
set(PLATFORM_LAYER_DIR ${PROJECT_SOURCE_DIR}/src/PlatformLayer)
set(RESOURCE_LAYER_DIR ${PROJECT_SOURCE_DIR}/src/ResourceLayer)
set(TOOL_LAYER_DIR ${PROJECT_SOURCE_DIR}/src/ToolLayer)

file(GLOB_RECURSE ALL_INCLUDE CONFIGURE_DEPENDS "*.h")
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS "*.cpp")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_INCLUDE} ${ALL_SOURCES})

add_executable(${TARGET_NAME} ${ALL_INCLUDE} ${ALL_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/ext)

target_link_libraries(${TARGET_NAME} PUBLIC embree)
target_link_libraries(${TARGET_NAME} PUBLIC Eigen3::Eigen)
target_link_libraries(${TARGET_NAME} PUBLIC pcg)

if (MESH_LOADER STREQUAL "Assimp")
    target_compile_definitions(${TARGET_NAME} PRIVATE MESH_LOADER_ASSIMP)
    target_link_libraries(${TARGET_NAME} PUBLIC assimp)
elseif (MESH_LOADER STREQUAL "Tinyobjloader")
    target_compile_definitions(${TARGET_NAME} PRIVATE MESH_LOADER_TINYOBJ)
    target_link_libraries(${TARGET_NAME} PUBLIC tinyobjloader)
endif()

if (USE_SAMPLED_SPECTRUM)
    target_compile_definitions(${TARGET_NAME} PRIVATE USING_SAMPLED_SPECTRUM)
endif()

target_compile_definitions(${TARGET_NAME} PRIVATE CMAKE_DEF_SPECTRUM_SAMPLES=60)

set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "_r")
