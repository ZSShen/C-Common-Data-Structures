cmake_minimum_required(VERSION 2.8)


# Define the constants to parse command options.
set(OPT_BUILD_DEBUG "Debug")
set(OPT_BUILD_RELEASE "Release")


# Define the constants for path generation.
set(PATH_INC "${CMAKE_CURRENT_SOURCE_DIR}")
set(PATH_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../lib")
set(PATH_BIN "${CMAKE_CURRENT_SOURCE_DIR}/../bin")

set(LIB_TYPE "SHARED")

set(HDR_DS "bin_search_tree.h")
set(SRC_DS "bin_search_tree.c")
set(SRC_DEMO "sample_usage.c")
set(OUT_DS "bstree")
set(OUT_DEMO "sample_usage")
set(TGE_DS "BIN_SEARCH_TREE")
set(TGE_DEMO "BIN_SEARCH_DEMO")


# Determine the build type and prepare the C macro if necessary.
if (CMAKE_BUILD_TYPE STREQUAL OPT_BUILD_DEBUG)
    set(PATH_OUT "${PATH_LIB}/debug")
    add_definitions(-DDEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL OPT_BUILD_RELEASE)
    set(PATH_OUT "${PATH_LIB}/release")
else()
    message("Error: CMAKE_BUILD_TYPE is not properly specified.")
    return()
endif()


# List the source building rules.
include_directories(${PATH_INC})

add_library(${TGE_DS} ${LIB_TYPE} ${SRC_DS})
set_target_properties(${TGE_DS} PROPERTIES
    PUBLIC_HEADER ${HDR_DS}
    LIBRARY_OUTPUT_DIRECTORY ${PATH_OUT}
    OUTPUT_NAME ${OUT_DS}
)

add_executable(${TGE_DEMO} ${SRC_DEMO})
target_link_libraries(${TGE_DEMO} ${TGE_DS})
set_target_properties(${TGE_DEMO} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PATH_BIN}
    OUTPUT_NAME ${OUT_DEMO}
)

