cmake_minimum_required(VERSION 2.8)

#==================================================================#
#                The subroutines for specific task                 #
#==================================================================#
# This subroutine builds the specified data structure.
function(SUB_BUILD_SPECIFIC DS)
    # Build the library for the data structure.
    set(HEAD_DS "${DS}.h")
    set(SRC_DS "${DS}.c")
    set(PATH_SUB "${PATH_OUT}/sub")
    string(TOUPPER ${DS} TGE_DS)

    add_library(${TGE_DS} ${LIB_TYPE} ${SRC_DS})
    set_target_properties(${TGE_DS} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PATH_SUB}
        OUTPUT_NAME ${DS}
    )

    # Build the demo program for the data structure.
    set(NAME_DEMO "demo_${DS}")
    set(SRC_DEMO "${PATH_DEMO}/${NAME_DEMO}.c")
    string(TOUPPER ${NAME_DEMO} TGE_DEMO)

    add_executable(${TGE_DEMO} ${SRC_DEMO})
    target_link_libraries(${TGE_DEMO} ${TGE_DS})
    set_target_properties(${TGE_DEMO} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PATH_BIN}
        OUTPUT_NAME ${NAME_DEMO}
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

# By default, we build the statically linked shared library.
set(LIB_TYPE "SHARED")

# List all the supported data structures.
set(LIST_DS
    simple_tree                 # Basic Binary Search Tree
    vector                      # Dynamic Array
)

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

# By default, we build all the data structures. But we can
# still use the command argument to build the specific one.
include_directories(${PATH_INC})
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

