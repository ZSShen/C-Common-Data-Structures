cmake_minimum_required(VERSION 2.8)
project(C-Common-Data-Structures)


# If the build type is not explicitly specified, we apply release build by default.
set(BUILD_TYPE_REL "Release")
set(MSG_BUILD_TYPE "Default Release Build")
if (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE ${BUILD_TYPE_REL} CACHE STRING ${MSG_BUILD_TYPE} FORCE)
endif()


# Determine the build object.
# For "Library" option, we build the shared library for the data structure.
# For "Unit" option, we build the unit test for the data structure.
# For "Demo" option, we build the demo program for the data structure.
# If the option is not explicitly specified, we build all of the stuffs.
set(OBJ_DS_LIB "Library")
set(OBJ_DS_UNIT "Unit")
set(OBJ_DS_DEMO "Demo")
set(KNOB_DS_LIB)
set(KNOB_DS_UNIT)
set(KNOB_DS_DEMO)
if(BUILD_OBJECT)
    STRING(REGEX REPLACE ":" ";" LIST_OBJ ${BUILD_OBJECT})
    if (";${LIST_OBJ};" MATCHES ";${OBJ_DS_LIB};")
        set(KNOB_DS_LIB " ")
    endif()
    if (";${LIST_OBJ};" MATCHES ";${OBJ_DS_UNIT};")
        set(KNOB_DS_UNIT " ")
    endif()
    if (";${LIST_OBJ};" MATCHES ";${OBJ_DS_DEMO};")
        set(KNOB_DS_DEMO " ")
    endif()
else()
    set(KNOB_DS_LIB " ")
    # set(KNOB_DS_UNIT " ")
    set(KNOB_DS_DEMO " ")
endif()


# Build the data structure libraries.
if (KNOB_DS_LIB)
    set(DIR_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
    message("*** Build Library ***")
    add_subdirectory(${DIR_SRC})
endif()

# Build the corresponding unit tests.
if (KNOB_DS_UNIT)
    set(DIR_TEST "${CMAKE_CURRENT_SOURCE_DIR}/test")
    message("*** Build Unit Test ***")
    add_subdirectory(${DIR_TEST})
endif()

# Build the corresponding demo programs.
if (KNOB_DS_DEMO)
    set(DIR_DEMO "${CMAKE_CURRENT_SOURCE_DIR}/demo")
    message("*** Build Demo Program ***")
    add_subdirectory(${DIR_DEMO})
endif()


# Set the "make run" target.
set(TARGET_RUN "run")
set(SCRIPT "./auto.py")
add_custom_target ( ${TARGET_RUN}
    COMMAND ${SCRIPT}
    WORKING_DIRECTORY ${DIR_TEST}
)
