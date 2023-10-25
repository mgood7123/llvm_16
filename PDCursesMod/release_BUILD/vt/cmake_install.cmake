# Install script for directory: /home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/vt

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump-13")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/lib/vt/libpdcursesstatic.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/lib/vt" TYPE STATIC_LIBRARY FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/vt/libpdcursesstatic.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/firework" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/firework")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/vt/firework"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/vt/firework")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/vt" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/vt/firework")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/firework" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/firework")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/vt/firework")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/ozdemo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/ozdemo")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/vt/ozdemo"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/vt/ozdemo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/vt" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/vt/ozdemo")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/ozdemo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/ozdemo")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/vt/ozdemo")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/newtest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/newtest")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/vt/newtest"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/vt/newtest")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/vt" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/vt/newtest")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/newtest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/newtest")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/vt/newtest")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/ptest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/ptest")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/vt/ptest"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/vt/ptest")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/vt" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/vt/ptest")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/ptest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/ptest")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/vt/ptest")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/rain" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/rain")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/vt/rain"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/vt/rain")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/vt" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/vt/rain")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/rain" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/rain")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/vt/rain")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/testcurs" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/testcurs")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/vt/testcurs"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/vt/testcurs")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/vt" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/vt/testcurs")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/testcurs" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/testcurs")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/vt/testcurs")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/tuidemo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/tuidemo")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/vt/tuidemo"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/vt/tuidemo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/vt" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/vt/tuidemo")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/tuidemo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/tuidemo")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/vt/tuidemo")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/worm" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/worm")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/vt/worm"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/vt/worm")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/vt" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/vt/worm")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/worm" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/worm")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/vt/worm")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/xmas" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/xmas")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/vt/xmas"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/vt/xmas")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/vt" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/vt/xmas")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/vt/xmas" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/vt/xmas")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/vt/xmas")
    endif()
  endif()
endif()

