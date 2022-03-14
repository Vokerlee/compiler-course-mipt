 #########################################################################

# CMAKE MINIMAL VERSION
cmake_minimum_required (VERSION 3.8)

# PROJECT PROPERTIES
project ("Akinator" LANGUAGES CXX)

# WORKS ONLY ON WINDOWS

if (UNIX)
    message(FATAL_ERROR "this project cannot be assemled on this OS")
    return()
endif()

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

# AKINATOR

set(AKINATOR_NAME "akinator")

set(AKINATOR_SRC
    akinator.cpp
    main.cpp
    tree.cpp
)

add_executable(${AKINATOR_NAME} ${AKINATOR_SRC})

#########################################################################
