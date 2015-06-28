cmake_minimum_required(VERSION 2.8)


#==================================================================#
#                The subroutines for specific task                 #
#==================================================================#
# This subroutine builds the demo program for the specified data structure.
function(SUB_BUILD_SPECIFIC DS)
    set(NAME_DEMO "demo_${DS}")
    set(SRC_DEMO "${CMAKE_CURRENT_SOURCE_DIR}/${NAME_DEMO}.c")
    string(TOUPPER ${NAME_DEMO} TGE_DEMO)

    add_executable(${TGE_DEMO} ${SRC_DEMO})
    target_link_libraries(${TGE_DEMO} ${DS})
    set_target_properties(${TGE_DEMO} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PATH_BIN}
        OUTPUT_NAME ${NAME_DEMO}
    )
endfunction()

# This subroutine builds all the demo programs.
function(SUB_BUILD_ENTIRE)
    foreach(DS ${LIST_DS})
        SUB_BUILD_SPECIFIC(${DS})
    endforeach()
endfunction()


#==================================================================#
#                    The CMakeLists entry point                    #
#==================================================================#
# Define the constants to parse command options.
set(OPT_BUILD_DEBUG "Debug")
set(OPT_BUILD_RELEASE "Release")

# Define the constants for path generation.
set(PATH_INC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(PATH_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../lib")
set(PATH_BIN "${CMAKE_CURRENT_SOURCE_DIR}/../bin/demo")

# List all the supported data structures.
set(REGEX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
FILE(GLOB_RECURSE LIST_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${REGEX_SRC})
set(LIST_DS)
foreach(SRC ${LIST_SRC})
    STRING(REGEX REPLACE ".c$" "" DS ${SRC})
    STRING(REGEX REPLACE "^demo_" "" DS ${DS})
    set(LIST_DS ${LIST_DS} ${DS})
endforeach()

# Determine the build type and generate the corresponding library path.
if (CMAKE_BUILD_TYPE STREQUAL OPT_BUILD_DEBUG)
    set(PATH_LIB "${PATH_LIB}/debug/sub")
    add_definitions(-DDEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL OPT_BUILD_RELEASE)
    set(PATH_LIB "${PATH_LIB}/release/sub")
else()
    message("Error: CMAKE_BUILD_TYPE is not properly specified.")
    return()
endif()

include_directories(${PATH_INC})
link_directories(${PATH_LIB})

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
