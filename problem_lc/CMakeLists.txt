#------------------------------------------------------------------------------
#
# CMake approach
# * dependencies are all correct
# * additional library output
# * object targets and separate dir for free
#
#------------------------------------------------------------------------------
#
# This file is licensed after LGPL v3
# Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
#
#------------------------------------------------------------------------------

# rm -rf build
# cmake -DCMAKE_BUILD_TYPE=Release -S . -B build 
# cmake --build ./build
# env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test

cmake_minimum_required(VERSION 3.11)
project("problem_lc")

enable_testing()

SET(SRCS
  source/list.c
  source/cache.c
  source/hash.c
)

SET(TESTS ${CMAKE_SOURCE_DIR}/Tests)

add_library(lclib STATIC ${SRCS})
target_include_directories(lclib PUBLIC "include")

add_executable(LC.x LCmain.c)
target_link_libraries(LC.x lclib)

add_test(
  NAME run_externals
  COMMAND bash -c "${CMAKE_SOURCE_DIR}/runtests.sh ${TESTS}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties(run_externals PROPERTIES DEPENDS LC.x)