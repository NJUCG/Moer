cmake_minimum_required(VERSION 3.12)

project(Zero LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_EXTERN_INCLUDE "${PROJECT_SOURCE_DIR}/ext")
set(PROJECT_EXTERN_LIBDIR "${PROJECT_SOURCE_DIR}/extlib")
message(STATUS ${PROJECT_EXTERN_INCLUDE})

# ============================================================ test module
set(TEST_DIR ${PROJECT_SOURCE_DIR}/test/)

# add catch2 unit test lib
add_subdirectory(${PROJECT_EXTERN_INCLUDE}/catch2)
add_subdirectory(test)

enable_testing()

function(Testing)
    foreach(TestModuleName IN LISTS ARGN)
        add_test(NAME ${PROJECT_NAME}_${TestModuleName} COMMAND $<TARGET_FILE:${PROJECT_NAME}_${TestModuleName}>)
    endforeach()
endfunction(Testing)

Testing(
    test-integrator
    test-integrator-validate
    test-materials
    test-camera
    # add new test module name
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)