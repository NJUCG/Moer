cmake_minimum_required(VERSION 3.12)

set(TEST_NAME test-triangle)

AddTestModule(
    ModuleName ${TEST_NAME}
    Src Main.cpp
    ${ZERO_SOURCES}
)

set(EXCNAME ${PROJECT_NAME}_${TEST_NAME})

target_include_directories(${EXCNAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/ext)

if(MSVC AND ((NOT CMAKE_BUILD_TYPE) OR CMAKE_BUILD_TYPE STREQUAL "Debug") )    
    set_property(TARGET ${EXCNAME} APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")
endif()