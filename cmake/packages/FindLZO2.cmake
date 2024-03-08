# Try to find the LZO2 library
#
# If successful, the following variables will be defined:
# LZO2_INCLUDE_DIR
# LZO2_LIBRARIES
# LZO2_FOUND

find_package(PkgConfig QUIET)
pkg_check_modules(PC_LZO2 QUIET LZO2)

set(CMAKE_FIND_DEBUG_MODE TRUE)

find_path(LZO2_INCLUDE_DIRS NAMES lzo/lzoconf.h
  PATHS ${LLVM_BUILD_ROOT__ROOTFS}/include
  NO_DEFAULT_PATH
  NO_PACKAGE_ROOT_PATH
  NO_CMAKE_PATH
  NO_CMAKE_ENVIRONMENT_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_SYSTEM_PATH
  NO_CMAKE_FIND_ROOT_PATH
)
find_library(LZO2_LIBRARIES NAMES lzo2 liblzo2
  PATHS ${LLVM_BUILD_ROOT__ROOTFS}/lib
  NO_DEFAULT_PATH
  NO_PACKAGE_ROOT_PATH
  NO_CMAKE_PATH
  NO_CMAKE_ENVIRONMENT_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_SYSTEM_PATH
  NO_CMAKE_FIND_ROOT_PATH
)

set(CMAKE_FIND_DEBUG_MODE FALSE)

include(CheckIncludeFile)
if(LZO2_INCLUDE_DIRS AND EXISTS "${LZO2_INCLUDE_DIRS}/lzo/lzoconf.h")
  include(CMakePushCheckState)
  cmake_push_check_state()
  set(CMAKE_REQUIRED_QUIET ${LZO2_FIND_QUIETLY})
  set(CMAKE_REQUIRED_INCLUDES ${LZO2_INCLUDE_DIRS})
  set(CMAKE_REQUIRED_LIBRARIES ${LZO2_LIBRARIES})
  check_include_file(lzo/lzoconf.h HAVE_LZO2_H)
  cmake_pop_check_state()
  if (HAVE_LZO2_H)
    file(STRINGS "${LZO2_INCLUDE_DIRS}/lzo/lzoconf.h" _lzo_version_lines REGEX "#define[ \t]+LZO2_VERSION_STRING")
    string(REGEX REPLACE ".*LZO2_VERSION_STRING \"\([0-9]*\)\..*\"" "\\1" _lzo_version_major "${_lzo_version_lines}")
    string(REGEX REPLACE ".*LZO2_VERSION_STRING \"[0-9]*\.\([0-9]*\)\"" "\\1" _lzo_version_minor "${_lzo_version_lines}")
    set(LZO2_VERSION_STRING "${_lzo_version_major}.${_lzo_version_minor}")
  else()
    set(LZO2_INCLUDE_DIRS "")
    set(LZO2_LIBRARIES "")
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LZO2
                                  FOUND_VAR
                                    LZO2_FOUND
                                  REQUIRED_VARS
                                    LZO2_INCLUDE_DIRS
                                    LZO2_LIBRARIES
                                  VERSION_VAR
                                    LZO2_VERSION_STRING)
mark_as_advanced(LZO2_INCLUDE_DIRS LZO2_LIBRARIES)

message(STATUS "LZO2: found :        ${LZO2_FOUND}")
message(STATUS "LZO2: include_dirs : ${LZO2_INCLUDE_DIRS}")
message(STATUS "LZO2: lib :          ${LZO2_LIBRARIES}")
message(STATUS "LZO2: version :      ${LZO2_VERSION_STRING}")

if (LZO2_FOUND AND NOT TARGET LLVM_STATIC_LZO2)
  add_library(LLVM_STATIC_LZO2 UNKNOWN IMPORTED)
  set_target_properties(LLVM_STATIC_LZO2 PROPERTIES
                        IMPORTED_LOCATION ${LZO2_LIBRARIES}
                        INTERFACE_INCLUDE_DIRECTORIES ${LZO2_INCLUDE_DIRS})
  set(LZO2_TARGET LLVM_STATIC_LZO2)
endif()
