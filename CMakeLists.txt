cmake_minimum_required(VERSION 3.5)

project(llvm_mcjit C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT COMMAND testBuilder_build)
    add_subdirectory(testBuilder)
    testBuilder_set_current_working_directory_to_default_binary_directory()
endif()

unset(LIBRARY_LLVM_18_STATIC_NEEDS_GENERATION)
unset(LIBRARY_LLVM_18_STATIC_NEEDS_GENERATION CACHE)

unset(LIBRARY_LLVM_18_STATIC)
unset(LIBRARY_LLVM_18_STATIC CACHE)
find_library(
  LIBRARY_LLVM_18_STATIC
  NAMES LLVM_18_STATIC
  PATHS ${CMAKE_BINARY_DIR}
  NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH
  NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH
  NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH
  NO_CMAKE_FIND_ROOT_PATH
)
if ("${LIBRARY_LLVM_18_STATIC}" STREQUAL "LIBRARY_LLVM_18_STATIC-NOTFOUND")
  message(STATUS "LLVM_18_STATIC NOT FOUND, generating")
  set(LIBRARY_LLVM_18_STATIC_NEEDS_GENERATION TRUE)
else()
  unset(LIBRARY_CLANG_18_STATIC)
  unset(LIBRARY_CLANG_18_STATIC CACHE)
  find_library(
    LIBRARY_CLANG_18_STATIC
    NAMES CLANG_18_STATIC
    PATHS ${CMAKE_BINARY_DIR}
    NO_DEFAULT_PATH NO_PACKAGE_ROOT_PATH
    NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH
    NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_FIND_ROOT_PATH
  )
  if ("${LIBRARY_CLANG_18_STATIC}" STREQUAL "LIBRARY_CLANG_18_STATIC-NOTFOUND")
    message(STATUS "CLANG_18_STATIC NOT FOUND, generating")
    set(LIBRARY_LLVM_18_STATIC_NEEDS_GENERATION TRUE)
  else()
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/LLVM_18_STATIC.target_link_libraries)
      message(WARNING "LLVM_18_STATIC.target_link_libraries (list of required system libs) NOT FOUND, generating")
      set(LIBRARY_LLVM_18_STATIC_NEEDS_GENERATION TRUE)
    else()
      set(LIBRARY_LLVM_18_STATIC_NEEDS_GENERATION FALSE)
    endif()
  endif()
endif()

if (LIBRARY_LLVM_18_STATIC_NEEDS_GENERATION)

  # - static
  #
  # -  :$<TARGET_OBJECTS:objLib>
  #
  # du -shc ./release_BUILD/llvm/lib/* | grep -e "LLVM.*\.a" | sed -e "s/.*libLLVM/list\(APPEND LLVM_MC_JIT_LINK_LIBS LLVM/g" -e "s/\.a/\)/g" > llvm_libs.cmake
  # du -shc ./release_BUILD/llvm/lib/* | grep -e "clang.*\.a" | sed -e "s/.*libclang/list\(APPEND LLVM_MC_JIT_LINK_LIBS clang/g" -e "s/\.a/\)/g" > clang_libs.cmake

  list(APPEND LLVM_MC_JIT_LINK_LIBS LLVMSupportBlake3)
  list(APPEND LLVM_MC_JIT_LINK_LIBS libclang)

  include(./llvm_libs.cmake)
  include(./clang_libs.cmake)

  if(CMAKE_SYSTEM_NAME MATCHES "Linux" )

    include(CheckIncludeFile)
    check_include_file("opagent.h" HAVE_OPAGENT_H)

    if (HAVE_OPAGENT_H)
      set(LLVM_USE_OPROFILE ON CACHE BOOL "" FORCE)
      list(APPEND LLVM_MC_JIT_LINK_LIBS LLVMOProfileJIT)
    endif()

    set(LLVM_USE_PERF ON CACHE BOOL "" FORCE)
    list(APPEND LLVM_MC_JIT_LINK_LIBS LLVMPerfJITEvents)

  endif()

  if(CMAKE_SYSTEM_NAME MATCHES "Windows" OR CMAKE_SYSTEM_NAME MATCHES "Linux" )
    set(LLVM_USE_INTEL_JITEVENTS ON CACHE BOOL "" FORCE)
    list(APPEND LLVM_MC_JIT_LINK_LIBS LLVMIntelJITEvents)
  endif()


  # shared llvm is supported but shared clang is not supported
  # see https://github.com/llvm/llvm-project/issues/46461
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "" FORCE)
  set(LLVM_INCLUDE_RUNTIMES OFF CACHE BOOL "" FORCE)
  set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(LLVM_INCLUDE_BENCHMARKS OFF CACHE BOOL "" FORCE)
  set(LLVM_INCLUDE_DOCS OFF CACHE BOOL "" FORCE)
  # LLVM_BUILD_TOOLS is required for to build clang
  set(LLVM_BUILD_TOOLS ON CACHE BOOL "" FORCE)
  set(LLVM_BUILD_UTILS OFF CACHE BOOL "" FORCE)
  set(LLVM_BUILD_RUNTIMES OFF CACHE BOOL "" FORCE)
  set(LLVM_BUILD_RUNTIME OFF CACHE BOOL "" FORCE)
  set(LLVM_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(LLVM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(LLVM_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
  set(LLVM_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(LLVM_ENABLE_DOXYGEN OFF CACHE BOOL "" FORCE)
  set(LLVM_ENABLE_SPHINX OFF CACHE BOOL "" FORCE)
  set(LLVM_ENABLE_OCAMLDOC OFF CACHE BOOL "" FORCE)
  set(LLVM_ENABLE_BINDINGS OFF CACHE BOOL "" FORCE)
  set(LLVM_BUILD_EXTERNAL_COMPILER_RT OFF CACHE BOOL "" FORCE)
  set(LLVM_VERSION_PRINTER_SHOW_HOST_TARGET_INFO ON CACHE BOOL "" FORCE)
  set(LLVM_BUILD_LLVM_C_DYLIB OFF CACHE BOOL "" FORCE)
  set(LLVM_OPTIMIZED_TABLEGEN ON CACHE BOOL "" FORCE)
  set(LLVM_ENABLE_EH ON CACHE BOOL "" FORCE)
  set(LLVM_ENABLE_RTTI ON CACHE BOOL "" FORCE)
  set(LLVM_ENABLE_PROJECTS "clang;lldb" CACHE STRING "" FORCE)

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LLVM_UNREACHABLE_OPTIMIZE OFF CACHE BOOL "" FORCE)
  endif()

  set(CLANG_ENABLE_ARCMT OFF CACHE BOOL "" FORCE)
  set(CLANG_ENABLE_STATIC_ANALYZER OFF CACHE BOOL "" FORCE)
  set(CLANG_ENABLE_PROTO_FUZZER OFF CACHE BOOL "" FORCE)
  set(CLANG_ENABLE_HLSL OFF CACHE BOOL "" FORCE)
  set(CLANG_INCLUDE_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(CLANG_INCLUDE_TOOLS OFF CACHE BOOL "" FORCE)
  set(CLANG_INCLUDE_DOCS OFF CACHE BOOL "" FORCE)
  set(CLANG_INCLUDE_TESTS OFF CACHE BOOL "" FORCE)
  set(CLANG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(CLANG_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
  set(CLANG_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(CLANG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(CLANG_ENABLE_BOOTSTRAP OFF CACHE BOOL "" FORCE)

  add_subdirectory(llvm)

  foreach(lib IN ITEMS ${LLVM_MC_JIT_LINK_LIBS})
    if (TARGET ${lib})
      if (${lib} MATCHES "^LLVM")
        list(APPEND LLVM_MC_JIT_LINK_LIBS_LLVM ${lib})
        if (TARGET obj.${lib})
          list(APPEND LLVM_18_OBJECT_FILES $<TARGET_OBJECTS:obj.${lib}>)
        else()
          list(APPEND LLVM_18_OBJECT_FILES $<TARGET_OBJECTS:${lib}>)
        endif()
        get_target_property(OUT ${lib} LLVM_SYSTEM_LIBS)
        if (NOT OUT MATCHES "NOTFOUND$")
          foreach(link_lib IN ITEMS ${OUT})
            if (link_lib MATCHES "^clang")
            elseif (link_lib MATCHES "^LLVM")
            elseif (link_lib MATCHES "^@")
            elseif (link_lib MATCHES "^:")
            elseif (link_lib MATCHES ".*::.*")
            else()
              list(APPEND LLVM_18_L ${link_lib})
            endif()
          endforeach()
        else()
          get_target_property(OUT ${lib} LINK_LIBRARIES)
          if (NOT OUT MATCHES "NOTFOUND$")
            foreach(link_lib IN ITEMS ${OUT})
              if (link_lib MATCHES "^clang")
              elseif (link_lib MATCHES "^LLVM")
              elseif (link_lib MATCHES "^@")
              elseif (link_lib MATCHES "^:")
              elseif (link_lib MATCHES ".*::.*")
              else()
                list(APPEND LLVM_18_L ${link_lib})
              endif()
            endforeach()
          endif()
        endif()
      else()
        list(APPEND LLVM_MC_JIT_LINK_LIBS_CLANG ${lib})
        if (TARGET obj.${lib})
          list(APPEND CLANG_18_OBJECT_FILES $<TARGET_OBJECTS:obj.${lib}>)
        else()
          list(APPEND CLANG_18_OBJECT_FILES $<TARGET_OBJECTS:${lib}>)
        endif()
      endif()
    endif()
  endforeach()

  list(REMOVE_DUPLICATES LLVM_18_L)

  message(STATUS "linking against these LLVM libraries : ${LLVM_MC_JIT_LINK_LIBS_LLVM}")
  message(STATUS "linking against these CLANG libraries : ${LLVM_MC_JIT_LINK_LIBS_CLANG}")
  message(STATUS "linking against these system libraries : ${LLVM_18_L}")

  add_library(LLVM_18_STATIC STATIC empty.c ${LLVM_18_OBJECT_FILES})
  target_include_directories(LLVM_18_STATIC PUBLIC ${CMAKE_BINARY_DIR}/llvm/include)
  target_include_directories(LLVM_18_STATIC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/llvm/include)
  target_link_libraries(LLVM_18_STATIC PUBLIC ${LLVM_18_L})
  file(WRITE ${CMAKE_BINARY_DIR}/LLVM_18_STATIC.target_link_libraries "${LLVM_18_L}")

  add_library(CLANG_18_STATIC STATIC empty.c ${CLANG_18_OBJECT_FILES})
  target_include_directories(CLANG_18_STATIC PUBLIC ${CMAKE_BINARY_DIR}/llvm/tools/clang/include)
  target_include_directories(CLANG_18_STATIC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/clang/include)
  target_link_libraries(CLANG_18_STATIC PUBLIC LLVM_18_STATIC)
else()
  add_library(LLVM_18_STATIC STATIC IMPORTED)
  set_target_properties(LLVM_18_STATIC PROPERTIES IMPORTED_LOCATION ${LIBRARY_LLVM_18_STATIC})
  target_include_directories(LLVM_18_STATIC INTERFACE ${CMAKE_BINARY_DIR}/llvm/include)
  target_include_directories(LLVM_18_STATIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/llvm/include)
  file(READ ${CMAKE_BINARY_DIR}/LLVM_18_STATIC.target_link_libraries LLVM_18_L)
  target_link_libraries(LLVM_18_STATIC INTERFACE ${LLVM_18_L})

  add_library(CLANG_18_STATIC STATIC IMPORTED)
  set_target_properties(CLANG_18_STATIC PROPERTIES IMPORTED_LOCATION ${LIBRARY_CLANG_18_STATIC})
  target_include_directories(CLANG_18_STATIC INTERFACE ${CMAKE_BINARY_DIR}/llvm/tools/clang/include)
  target_include_directories(CLANG_18_STATIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/clang/include)
  target_link_libraries(CLANG_18_STATIC INTERFACE LLVM_18_STATIC)
endif()

add_subdirectory(clang_driver)
add_subdirectory(replxx)
set(ARGPARSE_INSTALL OFF CACHE BOOL "" FORCE)
set(ARGPARSE_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ARGPARSE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(argparse)

add_library(llvm_orcjit STATIC src/jit.cpp)
target_include_directories(llvm_orcjit PUBLIC include)
target_link_libraries(llvm_orcjit clang_driver)

set(PDC_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PDC_SDL2_BUILD OFF CACHE BOOL "" FORCE)
set(PDC_SDL2_DEPS_BUILD OFF CACHE BOOL "" FORCE)
set(PDC_GL_BUILD OFF CACHE BOOL "" FORCE)
add_subdirectory(PDCursesMod)

add_library(pdcurses STATIC empty.c)
target_link_libraries(pdcurses vt_pdcursesstatic)
target_include_directories(pdcurses PUBLIC PDCursesMod)

add_subdirectory(minizip)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gen.zip
    COMMAND ${CMAKE_COMMAND} CC=clang CXX=clang++ -B 
    DEPENDS gen
    VERBATIM
)
add_dependencies(test1 test2_run)

add_executable(llvm_orcjit_example src/example.cpp)
target_link_libraries(llvm_orcjit_example llvm_orcjit replxx argparse pdcurses)
target_compile_options(replxx PRIVATE "-O0")
target_compile_options(replxx PRIVATE "-g3")
target_compile_options(llvm_orcjit_example PRIVATE "-O0")
target_compile_options(llvm_orcjit_example PRIVATE "-g3")
