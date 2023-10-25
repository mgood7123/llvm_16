# Install script for directory: /home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/ncurses

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
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/firework" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/firework")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/firework"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/ncurses/firework")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/ncurses" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/ncurses/firework")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/firework" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/firework")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/firework")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/ozdemo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/ozdemo")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/ozdemo"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/ncurses/ozdemo")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/ncurses" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/ncurses/ozdemo")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/ozdemo" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/ozdemo")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/ozdemo")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/newtest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/newtest")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/newtest"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/ncurses/newtest")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/ncurses" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/ncurses/newtest")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/newtest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/newtest")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/newtest")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/rain" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/rain")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/rain"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/ncurses/rain")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/ncurses" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/ncurses/rain")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/rain" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/rain")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/rain")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/testcurs" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/testcurs")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/testcurs"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/ncurses/testcurs")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/ncurses" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/ncurses/testcurs")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/testcurs" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/testcurs")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/testcurs")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/worm" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/worm")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/worm"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/ncurses/worm")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/ncurses" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/ncurses/worm")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/worm" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/worm")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/worm")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "applications" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/xmas" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/xmas")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/xmas"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/Release/bin/ncurses/xmas")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/Release/bin/ncurses" TYPE EXECUTABLE FILES "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit/PDCursesMod/release_BUILD/ncurses/xmas")
  if(EXISTS "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/xmas" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/xmas")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip-13" "$ENV{DESTDIR}/usr/local/Release/bin/ncurses/xmas")
    endif()
  endif()
endif()

