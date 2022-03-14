 #########################################################################

# CMAKE MINIMAL VERSION
cmake_minimum_required (VERSION 3.8)

# PROJECT PROPERTIES
project ("Eugene Onegin" LANGUAGES CXX)

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

# SOLVER

set(ONEGIN_NAME "onegin_sorter")

set(ONEGIN_SRC
    "Eugene Onegin.cpp"
    bred.cpp
    tests.cpp
    sort_func.cpp
    file_sort.cpp
)

add_executable(${ONEGIN_NAME} ${ONEGIN_SRC})

#########################################################################

