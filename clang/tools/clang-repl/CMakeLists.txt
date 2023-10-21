set( LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  Core
  LineEditor
  Option
  OrcJIT
  Support
  AArch64AsmParser
  AArch64CodeGen
  AArch64Desc
  AArch64Disassembler
  AArch64Info
  AArch64Utils
  ARMAsmParser
  ARMCodeGen
  ARMDesc
  ARMDisassembler
  ARMInfo
  ARMUtils
  X86AsmParser
  X86CodeGen
  X86Desc
  X86Disassembler
  X86Info
  X86TargetMCA
  )

add_clang_tool(clang-repl
  ClangRepl.cpp
  )

clang_target_link_libraries(clang-repl PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangInterpreter
  )

# Support plugins.
if(CLANG_PLUGIN_SUPPORT)
  export_executable_symbols_for_plugins(clang-repl)
endif()
