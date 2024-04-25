#------------------------------------------------------------------------------
#
# CMake build system for test runs
#
#------------------------------------------------------------------------------
#
# This file is licensed after LGPL v3
# Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
#
#------------------------------------------------------------------------------

set(TEST_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/test-run.rb")

function(add_testrun TGT APP)
  set(TEST_TARGET ${TGT}_test)
  set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${TGT})

  # variable ${APP}_app is defined in apps\CMakeLists.txt
  set(APP_VAR ${${APP}_app})

  # see readbst-back example
  string(REPLACE ";" " " ARGN_FLAGS_STR "${ARGN}")

  # and now bring it together
  add_test(
    NAME ${TEST_TARGET}
    COMMAND bash -c "${TEST_SCRIPT} -t ${APP_VAR} -f ${TEST_DIR} ${ARGN_FLAGS_STR}"
    WORKING_DIRECTORY ${TEST_DIR})
  set_tests_properties(${TEST_TARGET} PROPERTIES DEPENDS ${APP})
endfunction()

# basic trees
add_testrun(readbst readbst)
add_testrun(readbst-back readbst "-a" "-back")
add_testrun(readbst-topo readbst "-a" "-topo")
add_testrun(readbst-ibrac readbst "-a" "-ibrac")
add_testrun(readbst-batch readbst "-a" "-batch" "-a" "-topo")

# permutations
add_testrun(permloops permloops)
add_testrun(invperm invperm)
add_testrun(allperms allperms)
add_testrun(baxters is-baxters)
add_testrun(baxters-batch is-baxters "-a" "-batch" "-a" "-combine")
add_testrun(baxters-filt is-baxters "-a" "-batch" "-a" "-filter")

# twin trees
add_testrun(readtwin readtwin)
add_testrun(readtwin-baxters readtwin "-a" "-baxters")

# braces
add_testrun(allbraces allbraces)

# subsets
add_testrun(allsubsets allsubsets)
