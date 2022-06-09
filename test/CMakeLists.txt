cmake_minimum_required(VERSION 3.12)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TEST_DIR}/target/)

set(CMAKE_CXX_STANDARD 17)

include(Testing.cmake)

add_subdirectory(example/test1)
add_subdirectory(example/test2)
#add_subdirectory(test-env)
add_subdirectory(test-integrator)
add_subdirectory(test-materials)
