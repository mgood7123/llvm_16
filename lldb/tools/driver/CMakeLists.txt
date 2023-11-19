set(LLVM_TARGET_DEFINITIONS Options.td)
tablegen(LLVM Options.inc -gen-opt-parser-defs)
add_public_tablegen_target(LLDBOptionsTableGen)

if(APPLE)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/lldb-Info.plist.in
    ${CMAKE_CURRENT_BINARY_DIR}/lldb-Info.plist
    )
  # Inline info plist in binary (use target_link_options for this as soon as CMake 3.13 is available)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-sectcreate,__TEXT,__info_plist,${CMAKE_CURRENT_BINARY_DIR}/lldb-Info.plist")
endif()

add_lldb_tool(lldb
  Driver.cpp

  # ifdef header guard required to compile only in lldb_driver_main
  driver_main.cpp

  Platform.cpp

  LINK_LIBS
    liblldbAPI

  LINK_COMPONENTS
    Option
    Support
  )

# we have a difficult time compiling Driver.cpp outside lldb source
# so we must build inside lldb source so we compile correctly
add_lldb_library(lldb_driver_main NO_PLUGIN_DEPENDENCIES
  driver_main.cpp
  Platform.cpp

  LINK_LIBS
    lldbCommands
    lldbCore
    lldbDataFormatters
    lldbHost
    lldbTarget
    lldbUtility

  LINK_COMPONENTS
    Support
  )

target_compile_definitions(lldb_driver_main PRIVATE LLDB_DRIVER_MAIN_LIBRARY_ENABLE)

add_dependencies(lldb_driver_main
  LLDBOptionsTableGen
  ${tablegen_deps}
)

add_dependencies(lldb
  LLDBOptionsTableGen
  ${tablegen_deps}
)



set_target_properties(LLDBOptionsTableGen PROPERTIES FOLDER "lldb misc")

if(LLDB_BUILD_FRAMEWORK)
  # In the build-tree, we know the exact path to the framework directory.
  # The installed framework can be in different locations.
  lldb_setup_rpaths(lldb
    BUILD_RPATH
      "${LLDB_FRAMEWORK_ABSOLUTE_BUILD_DIR}"
    INSTALL_RPATH
      "@loader_path/../../../SharedFrameworks"
      "@loader_path/../../System/Library/PrivateFrameworks"
      "@loader_path/../../Library/PrivateFrameworks"
  )
endif()
