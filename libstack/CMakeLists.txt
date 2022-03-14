 #########################################################################

# CMAKE MINIMAL VERSION
cmake_minimum_required (VERSION 3.8)

# PROJECT PROPERTIES
project ("Stack" LANGUAGES CXX)

#########################################################################

# ASSEMBLY OPTIONS

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_compile_options(
    -Wall
    -Wpedantic

    -Wno-int-to-pointer-cast
    -Wno-variadic-macros
    -Wno-unused-variable
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-sign-compare
    -Wno-misleading-indentation
    -Wno-unused-but-set-variable
)

#########################################################################

# TEXT LIBRARY

include_directories("../libtext")

# STACK

set(STACK_LIB_NAME "stack")

set(STACK_LIB_SRC
    stack_func.cpp
)

add_library(${STACK_LIB_NAME} STATIC)
target_sources(${STACK_LIB_NAME} PRIVATE ${STACK_LIB_SRC})

#########################################################################
