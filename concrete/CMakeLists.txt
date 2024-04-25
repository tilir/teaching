#------------------------------------------------------------------------------
#
# CMake build system for computability experiments
#
#------------------------------------------------------------------------------
#
# This file is licensed after LGPL v3
# Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
#
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)
project(computability)
enable_testing()

add_library(frame INTERFACE)
target_include_directories(frame INTERFACE framework)

add_library(comblib INTERFACE)
target_include_directories(comblib INTERFACE combinatorial)

add_subdirectory(apps)

add_subdirectory(tests)

install()
