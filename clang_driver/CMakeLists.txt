cmake_minimum_required(VERSION 3.20)

project(clang_driver C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(TempFile)

add_library(clang_driver STATIC
  driver/driver.cpp
  driver/cc1_main.cpp
  driver/cc1as_main.cpp
  driver/cc1gen_reproducer_main.cpp
  driver/ClangFormat.cpp
  default_formatting_style.cpp
)

target_include_directories(clang_driver PUBLIC include)

target_link_libraries(clang_driver LLVM_18_STATIC CLANG_18_STATIC LLDB_18_STATIC tmpfile)

target_compile_options(tmpfile PRIVATE "-O0")
target_compile_options(tmpfile PRIVATE "-g3")
target_compile_options(clang_driver PRIVATE "-O0")
target_compile_options(clang_driver PRIVATE "-g3")
