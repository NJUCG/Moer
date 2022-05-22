cmake_minimum_required(VERSION 3.12)

project(Zero LANGUAGES CXX)

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
    example-test1
    example-test2
    test-env
    test-texture
    # add new test module name
)