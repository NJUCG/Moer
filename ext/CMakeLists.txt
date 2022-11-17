cmake_minimum_required(VERSION 3.14)
set(3DPARTY_FOLDER ext)

function(set_ide_folder target_name folder_name)
    if (TARGET ${target_name})
        set_target_properties(${target_name} PROPERTIES FOLDER ${folder_name})
    endif()
endfunction()

include(FetchContent)

if (MESH_LOADER STREQUAL "Assimp")
    if (NOT TARGET assimp)
        message(STATUS "============start config assimp============")
        FetchContent_Declare(
            assimp
            GIT_REPOSITORY  https://github.com/assimp/assimp.git
            GIT_TAG         v5.2.5
        )

        set(ASSIMP_FOLDER ${3DPARTY_FOLDER}/assimp)
        set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE INTERNAL "")
        set(ASSIMP_DOUBLE_PRECISION ON CACHE INTERNAL "")
        set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE INTERNAL "")
        set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "")
        set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
        set(ASSIMP_INSTALL OFF CACHE INTERNAL "")

        FetchContent_MakeAvailable(assimp)
        
        set_target_properties(assimp PROPERTIES DEBUG_POSTFIX "_d")
        set_target_properties(assimp PROPERTIES RELEASE_POSTFIX "_r")

        set_ide_folder(assimp ${ASSIMP_FOLDER})
        set_ide_folder(UpdateAssimpLibsDebugSymbolsAndDLLs ${ASSIMP_FOLDER})
        set_ide_folder(zlibstatic ${ASSIMP_FOLDER})
    endif()
elseif (MESH_LOADER STREQUAL "Tinyobjloader")
    if (NOT TARGET tinyobjloader)
        message(STATUS "============start config tinyobjloader============")
        # add_subdirectory(tinyobjloader)
        FetchContent_Declare(
            tinyobjloader
            GIT_REPOSITORY  https://github.com/tinyobjloader/tinyobjloader.git
            GIT_TAG         v2.0.0rc10
        )
        FetchContent_MakeAvailable(tinyobjloader)
        
        target_include_directories(tinyobjloader PUBLIC $<BUILD_INTERFACE:${tinyobjloader_SOURCE_DIR}>)
        
        set_ide_folder(tinyobjloader ${3DPARTY_FOLDER}/tinyobjloader)
        set_ide_folder(uninstall ${3DPARTY_FOLDER}/tinyobjloader)
    endif()
else()
    message(FATAL_ERROR "Unknown mesh loader!!")
endif()

if (NOT TARGET Eigen3::Eigen)
    message(STATUS "============start config Eigen============")
    FetchContent_Declare(
        Eigen
        GIT_REPOSITORY  https://gitlab.com/libeigen/eigen.git
        GIT_TAG         3.4.0
    )
    
    set(EIGEN_BUILD_TESTING OFF CACHE INTERNAL "")
    set(EIGEN_BUILD_DOC OFF CACHE INTERNAL "")

    FetchContent_MakeAvailable(Eigen)
        
    set_target_properties(eigen PROPERTIES DEBUG_POSTFIX "_d")
    set_target_properties(eigen PROPERTIES RELEASE_POSTFIX "_r")

    find_package(Eigen3 3.3 REQUIRED NO_MODULE)
    set_ide_folder(eigen ${3DPARTY_FOLDER})
endif()

if (NOT TARGET embree)
    message(STATUS "============start config embree============")
    set(EMBREE_FOLDER ${3DPARTY_FOLDER}/embree)
    if(EMBREE_USE_TBB)
        set(EMBREE_TASKING_SYSTEM TBB)
    else()
        if(WIN32)
            set(EMBREE_TASKING_SYSTEM PPL)
        else()
            set(EMBREE_TASKING_SYSTEM INTERNAL)
        endif()
    endif()
    
    set(EMBREE_TUTORIALS OFF CACHE INTERNAL "")

    add_subdirectory(embree)
    set_target_properties(embree PROPERTIES DEBUG_POSTFIX "_d")
    set_target_properties(embree PROPERTIES RELEASE_POSTFIX "_r")

    set_ide_folder(embree ${EMBREE_FOLDER})
    set_ide_folder(uninstall ${EMBREE_FOLDER})
    set_ide_folder(lexers ${EMBREE_FOLDER}/common)
    set_ide_folder(math ${EMBREE_FOLDER}/common)
    set_ide_folder(simd ${EMBREE_FOLDER}/common)
    set_ide_folder(sys ${EMBREE_FOLDER}/common)
    set_ide_folder(tasking ${EMBREE_FOLDER}/common)
    set_ide_folder(embree_avx ${EMBREE_FOLDER}/kernels)
    set_ide_folder(embree_avx2 ${EMBREE_FOLDER}/kernels)
    set_ide_folder(embree_sse42 ${EMBREE_FOLDER}/kernels)
endif()

if (NOT TARGET pcg)
    message(STATUS "============start config pcg============")
    add_subdirectory(pcg)
    set(PCG_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/pcg/pcg-cpp/include)
    target_include_directories(pcg INTERFACE ${PCG_INCLUDE_DIR})
    set_ide_folder(pcg ${3DPARTY_FOLDER})
endif()

if (NOT TARGET nlohmann_json)
    message(STATUS "============start config nlohmann_json============")
    add_subdirectory(json)
    set_ide_folder(nlohmann_json ${3DPARTY_FOLDER})
endif()

if (NOT TARGET stb)
    message(STATUS "============start config stb============")
    add_subdirectory(stb)
    target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb)
    set_ide_folder(stb ${3DPARTY_FOLDER})
endif()
