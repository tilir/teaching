#------------------------------------------------------------------------------
#
# CMake build system for different applications
# set env like readbst_app for parent scopr to use in testing
#
#------------------------------------------------------------------------------
#
# This file is licensed after LGPL v3
# Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
#
#------------------------------------------------------------------------------

set(APPS
  allbraces
  allperms
  allsubsets
  invperm
  is-baxters
  randbraces
  randperm
  readbst
  readtwin
  permloops
)

foreach(APP ${APPS})
  set(${APP}_app ${CMAKE_BINARY_DIR}/apps/${APP} PARENT_SCOPE)
  add_executable(${APP} ${APP}.cc)
  target_compile_features(${APP} PRIVATE cxx_std_20)
  target_link_libraries(${APP} frame)
  target_link_libraries(${APP} comblib)
endforeach()
