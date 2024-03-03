#
# Copyright (c) 2024 Piotr Stradowski. All rights reserved.
# Software distributed under the permissive MIT License.
#

#
# Project Info
#

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
cmake_policy(VERSION 3.9)

include(CMake/CMakeShared.cmake)

#
# Project
#

project(QuakeSTrainer)

# Setting up custom properties and functions from "CMakeShared.cmake" file
custom_cmake_setup()

#
# Targets Configuration
#

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#
# Sources
#

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
get_all_target_files(FILES 0 ${SOURCE_DIR})

add_executable(QuakeStrainer "${FILES}")

#
# Dependencies
#

# CoreMatrixToolkit
set(CORE_MATRIX_TOOLKIT_DIR "internal/CoreMatrixToolkit")
add_subdirectory(${CORE_MATRIX_TOOLKIT_DIR})
target_link_libraries(QuakeStrainer PRIVATE CoreMatrixToolkit)
target_include_directories(QuakeStrainer PRIVATE ${CORE_MATRIX_TOOLKIT_DIR}/include)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT QuakeStrainer)