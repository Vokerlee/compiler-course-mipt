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

# AKINATOR

set(AKINATOR_NAME "akinator")

set(AKINATOR_SRC
    src/akinator.cpp
    src/main.cpp
    src/tree.cpp
)

add_executable(${AKINATOR_NAME} ${AKINATOR_SRC})

target_include_directories(${AKINATOR_NAME} PRIVATE  ${TEXT_LIB_DIR})
target_link_libraries(${AKINATOR_NAME} PRIVATE text)

#########################################################################
