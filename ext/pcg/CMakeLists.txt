set(
    PCG_SRC
    ${CMAKE_CURRENT_LIST_DIR}/pcg-cpp/include/pcg_extras.hpp
    ${CMAKE_CURRENT_LIST_DIR}/pcg-cpp/include/pcg_random.hpp
    ${CMAKE_CURRENT_LIST_DIR}/pcg-cpp/include/pcg_uint128.hpp
)

add_library(pcg INTERFACE ${PCG_SRC})