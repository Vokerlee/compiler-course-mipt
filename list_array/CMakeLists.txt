 #########################################################################

# CMAKE MINIMAL VERSION
cmake_minimum_required (VERSION 3.8)

# PROJECT PROPERTIES
project ("List (array type)" LANGUAGES CXX)

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

set(TEXT_LIB_DIR libtext)
include_directories("../libtext")
add_subdirectory("../libtext" ${TEXT_LIB_DIR})

# LIST LIRARY

set(LIST_LIB_NAME "listarr")

set(LIST_LIB_SRC
    list_func.cpp
)

add_library(${LIST_LIB_NAME} STATIC)
target_sources(${LIST_LIB_NAME} PRIVATE ${LIST_LIB_SRC})

add_executable(test_prog main.cpp)
target_link_libraries(test_prog PRIVATE listarr text)

#########################################################################

