cmake_minimum_required(VERSION 3.3)

project(llvm_mcjit C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_RUNTIMES OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_UTILS OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_RUNTIMES OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_RUNTIME OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_DOCS OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_DOXYGEN OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_SPHINX OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_OCAMLDOC OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_BINDINGS OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_EXTERNAL_COMPILER_RT OFF CACHE BOOL "" FORCE)
set(LLVM_VERSION_PRINTER_SHOW_HOST_TARGET_INFO ON CACHE BOOL "" FORCE)
set(LLVM_BUILD_LLVM_C_DYLIB OFF CACHE BOOL "" FORCE)
set(LLVM_OPTIMIZED_TABLEGEN ON CACHE BOOL "" FORCE)
set(LLVM_USE_OPROFILE ON CACHE BOOL "" FORCE)

if(CMAKE_SYSTEM_NAME MATCHES "Windows" OR CMAKE_SYSTEM_NAME MATCHES "Linux" )
  set(LLVM_USE_INTEL_JITEVENTS ON CACHE BOOL "" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(LLVM_UNREACHABLE_OPTIMIZE OFF CACHE BOOL "" FORCE)
endif()

add_library(llvm_mcjit_core STATIC empty.c)
target_include_directories(llvm_mcjit_core PUBLIC llvm/include)
target_include_directories(llvm_mcjit_core PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/llvm/include)

if(CMAKE_SYSTEM_NAME MATCHES "Linux" )
  set(LLVM_USE_PERF ON CACHE BOOL "" FORCE)
  include(CheckIncludeFile)
  check_include_file("opagent.h" HAVE_OPAGENT_H)
endif()

add_subdirectory(llvm)

if(CMAKE_SYSTEM_NAME MATCHES "Windows" OR CMAKE_SYSTEM_NAME MATCHES "Linux" )
  if( LLVM_USE_INTEL_JITEVENTS )
    target_link_libraries(llvm_mcjit_core PRIVATE LLVMIntelJITEvents)
  endif( LLVM_USE_INTEL_JITEVENTS )
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux" )
  if (HAVE_OPAGENT_H)
    target_link_libraries(llvm_mcjit_core PRIVATE LLVMOProfileJIT)
  endif()

  if( LLVM_USE_PERF )
    target_link_libraries(llvm_mcjit_core PRIVATE LLVMPerfJITEvents)
  endif( LLVM_USE_PERF )
endif()

target_link_libraries(llvm_mcjit_core PRIVATE
  LLVMSupportBlake3
  LLVMDemangle
  LLVMSupport
  LLVMTarget
  LLVMBinaryFormat
  LLVMCore
  LLVMIRPrinter
  LLVMIRReader
  LLVMWindowsDriver
  LLVMX86CodeGen
  LLVMX86AsmParser
  LLVMX86Disassembler
  LLVMX86TargetMCA
  LLVMARMUtils
  LLVMARMInfo
  LLVMARMDesc
  LLVMARMCodeGen
  LLVMARMAsmParser
  LLVMARMDisassembler
  LLVMAArch64Utils
  LLVMAArch64Info
  LLVMAArch64Desc
  LLVMAArch64CodeGen
  LLVMAArch64AsmParser
  LLVMAArch64Disassembler
  LLVMCodeGen
  LLVMSelectionDAG
  LLVMAsmPrinter
  LLVMMIRParser
  # NOTE: the 'llvm-tblgen' executable uses 'LLVMTableGenGlobalISel', NOT 'LLVMGlobalISel'
  LLVMGlobalISel
  LLVMTargetParser
  LLVMIPO
  LLVMScalar
  LLVMObjCARC
  LLVMMC
  LLVMMCDisassembler
  LLVMMCA
  LLVMOption
  LLVMFrontendOpenMP
  LLVMLinker
  LLVMVectorize
  LLVMObject
  LLVMObjectYAML
  LLVMInstrumentation
  LLVMCoroutines
  LLVMCFGuard
  LLVMDebugInfoDWARF
  LLVMDebugInfoMSF
  LLVMDebugInfoPDB
  LLVMDebuginfod
  LLVMDebugInfoGSYM
  LLVMDebugInfoCodeView
  LLVMDebugInfoLogicalView
  LLVMDWARFLinker
  #LLVMDWARFLinkerParallel # currently empty namespace in LLVM 16
  LLVMDWP # merges DWARF 5 Split DWARF .dwo files into .dwp (DWARF package files)
  LLVMCoverage
  LLVMInterfaceStub
  LLVMObjCopy
  LLVMBitReader
  LLVMBitWriter
  LLVMBitstreamReader
  LLVMProfileData
  LLVMAggressiveInstCombine
  LLVMInstCombine
  LLVMRemarks
  LLVMTransformUtils
  LLVMAnalysis
  LLVMPasses
  LLVMLTO
  LLVMLineEditor
  LLVMSymbolize
  LLVMTextAPI
  LLVMXRay
  LLVMAsmParser
  LLVMFrontendOpenMP
  LLVMFrontendOpenACC
  LLVMFrontendHLSL
  LLVMFileCheck
  LLVMWindowsManifest
  LLVMWindowsDriver
  LLVMExtensions
  LLVMExecutionEngine
  LLVMInterpreter
  LLVMJITLink
  LLVMMCJIT
  LLVMOrcShared
  LLVMOrcTargetProcess
  LLVMOrcJIT
  LLVMRuntimeDyld
)

add_executable(llvm_mcjit_example example.cpp)
target_link_libraries(llvm_mcjit_example llvm_mcjit_core)
