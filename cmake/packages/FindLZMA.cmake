# Try to find the LZMA library
#
# If successful, the following variables will be defined:
# LZMA_INCLUDE_DIR
# LZMA_LIBRARIES
# LZMA_FOUND

find_package(PkgConfig QUIET)
pkg_check_modules(PC_LZMA QUIET LZMA)

find_path(LZMA_INCLUDE_DIRS NAMES lzma.h HINTS ${PC_LZMA_INCLUDE_DIRS})
find_library(LZMA_LIBRARIES NAMES lzma HINTS ${PC_LZMA_LIBRARY_DIRS})

include(CheckIncludeFile)
if(LZMA_INCLUDE_DIRS AND EXISTS "${LZMA_INCLUDE_DIRS}/lzma.h")
  include(CMakePushCheckState)
  cmake_push_check_state()
  set(CMAKE_REQUIRED_QUIET ${LZMA_FIND_QUIETLY})
  set(CMAKE_REQUIRED_INCLUDES ${LZMA_INCLUDE_DIRS})
  set(CMAKE_REQUIRED_LIBRARIES ${LZMA_LIBRARIES})
  check_include_file(lzma.h HAVE_LZMA_H)
  cmake_pop_check_state()
  if (HAVE_LZMA_H)
    file(STRINGS "${LZMA_INCLUDE_DIRS}/lzma/version.h" _lzma_version_lines REGEX "#define[ \t]+LZMA_VERSION_(MAJOR|MINOR|PATCH)")
    string(REGEX REPLACE ".*LZMA_VERSION_MAJOR *\([0-9]*\).*" "\\1" _lzma_version_major "${_lzma_version_lines}")
    string(REGEX REPLACE ".*LZMA_VERSION_MINOR *\([0-9]*\).*" "\\1" _lzma_version_minor "${_lzma_version_lines}")
    string(REGEX REPLACE ".*LZMA_VERSION_PATCH *\([0-9]*\).*" "\\1" _lzma_version_patch "${_lzma_version_lines}")
    set(LZMA_VERSION_STRING "${_lzma_version_major}.${_lzma_version_minor}.${_lzma_version_patch}")
  else()
    set(LZMA_INCLUDE_DIRS "")
    set(LZMA_LIBRARIES "")
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LZMA
                                  FOUND_VAR
                                    LZMA_FOUND
                                  REQUIRED_VARS
                                    LZMA_INCLUDE_DIRS
                                    LZMA_LIBRARIES
                                  VERSION_VAR
                                    LZMA_VERSION_STRING)
mark_as_advanced(LZMA_INCLUDE_DIRS LZMA_LIBRARIES)

message(STATUS "LZMA: found :        ${LZMA_FOUND}")
message(STATUS "LZMA: include_dirs : ${LZMA_INCLUDE_DIRS}")
message(STATUS "LZMA: lib :          ${LZMA_LIBRARIES}")
message(STATUS "LZMA: version :      ${LZMA_VERSION_STRING}")

if (LZMA_FOUND AND NOT TARGET LLVM_STATIC_LZMA)
  add_library(LLVM_STATIC_LZMA UNKNOWN IMPORTED)
  set_target_properties(LLVM_STATIC_LZMA PROPERTIES
                        IMPORTED_LOCATION ${LZMA_LIBRARIES}
                        INTERFACE_INCLUDE_DIRECTORIES ${LZMA_INCLUDE_DIRS})
  set(LZMA_TARGET LLVM_STATIC_LZMA)
endif()
