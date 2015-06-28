cmake_minimum_required(VERSION 2.8)


#==================================================================#
#                The subroutines for specific task                 #
#==================================================================#
# This subroutine builds the library for the specified data structure.
function(SUB_BUILD_SPECIFIC DS)
    set(HEAD_DS "${DS}.h")
    set(SRC_DS "${DS}.c")
    set(PATH_SUB "${PATH_OUT}/sub")
    string(TOUPPER ${DS} TGE_DS)

    # Some advanced structures depend on the implementation of basic structures.
    # If it is necessary to link the dependency, the developer should explicitly
    # specify the dependent source files here.
    set(SRC_DEP_DS "")
    if (DS STREQUAL "ordered_map")
        set(SRC_DEP_DS "balanced_tree.c")
    elseif (DS STREQUAL "binary_heap")
        set(SRC_DEP_DS "vector.c")
    elseif (DS STREQUAL "priority_queue")
        set(SRC_DEP_DS "vector.c")
        set(SRC_DEP_DS ${SRC_DEP_DS} "binary_heap.c")
    endif()

    add_library(${TGE_DS} ${LIB_TYPE} ${SRC_DS} ${SRC_DEP_DS})
    set_target_properties(${TGE_DS} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PATH_SUB}
        OUTPUT_NAME ${DS}
    )
endfunction()

# This subroutine builds all the data structures.
function(SUB_BUILD_ENTIRE)
    foreach(DS ${LIST_DS})
        SUB_BUILD_SPECIFIC(${DS})
    endforeach()

    # Build the integration library.
    string(TOUPPER ${LIB_CDS} TGE_CDS)
    set(REGEX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
    file(GLOB_RECURSE LIST_SRC ${REGEX_SRC})
    add_library(${TGE_CDS} ${LIB_TYPE} ${LIST_SRC})
    set_target_properties(${TGE_CDS} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PATH_OUT}
        OUTPUT_NAME ${LIB_CDS}
    )
endfunction()

#==================================================================#
#                    The CMakeLists entry point                    #
#==================================================================#
# The integration library name.
set(LIB_CDS "cds")

# Define the constants to parse command options.
set(OPT_BUILD_DEBUG "Debug")
set(OPT_BUILD_RELEASE "Release")

# Define the constants for path generation.
set(PATH_INC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(PATH_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../lib")
set(PATH_BIN "${CMAKE_CURRENT_SOURCE_DIR}/../bin")
set(PATH_DEMO "${CMAKE_CURRENT_SOURCE_DIR}/../demo")

# By default, we build the dynamically linked shared library.
set(LIB_TYPE "SHARED")

# List all the supported data structures.
set(REGEX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
FILE(GLOB_RECURSE LIST_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${REGEX_SRC})
set(LIST_DS)
foreach(SRC ${LIST_SRC})
    STRING(REGEX REPLACE ".c$" "" DS ${SRC})
    set(LIST_DS ${LIST_DS} ${DS})
endforeach()

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

include_directories(${PATH_INC})

# By default, we build the libraries for all the data structures. But we can
# use the command option to build the one for a specific structure.
if (BUILD_SOURCE)
    if (";${LIST_DS};" MATCHES ";${BUILD_SOURCE};")
        SUB_BUILD_SPECIFIC(${BUILD_SOURCE})
    else()
        message("Error: Invalid source file name.")
    endif()
else()
    SUB_BUILD_ENTIRE()
    return()
endif()

