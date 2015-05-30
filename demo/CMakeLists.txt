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
set(LIST_DS
    simple_tree                 # Basic Binary Search Tree
    balanced_tree               # Balanced Binary Search Tree
    vector                      # Dynamic Array
    dlinked_list                # Doubly Linked List
    ordered_map                 # Ordered Map
)

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

# By default, we build the demo programs for all the data structures. But we can
# still use the command argument to build the one for specific structure.
include_directories(${PATH_INC})
link_directories(${PATH_LIB})
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
