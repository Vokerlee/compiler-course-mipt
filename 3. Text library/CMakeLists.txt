#########################################################################

# CMAKE MINIMAL VERSION
cmake_minimum_required (VERSION 3.8)

# PROJECT PROPERTIES
project ("Text library" LANGUAGES CXX)

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

set(TEXT_LIB_NAME "text")

set(TEXT_LIB_SRC
    text_func.cpp
)

add_library(${TEXT_LIB_NAME} STATIC)
target_include_directories(${TEXT_LIB_NAME} PRIVATE text_lib)
target_sources(${TEXT_LIB_NAME} PRIVATE ${TEXT_LIB_SRC})

#########################################################################
