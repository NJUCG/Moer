function(AddTestModule)

    set(options Discard)
    set(oneValueArgs ModuleName)
    set(multiValueArgs Src)

    cmake_parse_arguments(M "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # message(STATUS "ModuleName is " ${M_ModuleName})
    # message(STATUS "Src are " ${M_Src})

    add_executable(
        ${PROJECT_NAME}_${M_ModuleName}
        ${M_Src}
    )

    target_include_directories(${PROJECT_NAME}_${M_ModuleName} PUBLIC ${PROJECT_SOURCE_DIR}/src)

    target_link_libraries(${PROJECT_NAME}_${M_ModuleName} Catch2::Test)

endfunction()
