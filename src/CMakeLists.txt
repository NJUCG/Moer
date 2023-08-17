set(TARGET_NAME Moer)

file(GLOB_RECURSE ALL_INCLUDE CONFIGURE_DEPENDS "*.h")
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS "*.cpp")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_INCLUDE} ${ALL_SOURCES})

add_executable(${TARGET_NAME} ${ALL_INCLUDE} ${ALL_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/ext/FastMath)
target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/ext)


add_dependencies(${TARGET_NAME} ext-copy)
target_link_libraries(${TARGET_NAME} PUBLIC Moer-ext)

find_package(Threads)
target_link_libraries(${TARGET_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})

# Better to use TBB
if (NOT MSVC)
    find_package(OpenMP REQUIRED)
    include_directories(OpenMP_CXX_INCLUDE_DIRS)
    target_link_libraries(${TARGET_NAME} PUBLIC OpenMP::OpenMP_CXX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
endif()

if (MESH_LOADER STREQUAL "Assimp")
    target_compile_definitions(${TARGET_NAME} PRIVATE MESH_LOADER_ASSIMP)
elseif (MESH_LOADER STREQUAL "Tinyobjloader")
    target_compile_definitions(${TARGET_NAME} PRIVATE MESH_LOADER_TINYOBJ)
endif()

if (USE_SAMPLED_SPECTRUM)
    target_compile_definitions(${TARGET_NAME} PRIVATE USING_SAMPLED_SPECTRUM)
endif()

target_compile_definitions(${TARGET_NAME} PRIVATE CMAKE_DEF_SPECTRUM_SAMPLES=60)

set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "_r")
