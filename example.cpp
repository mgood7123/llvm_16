//===----- ExampleModules.h - IR modules for LLJIT examples -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Example modules for LLJIT examples
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_EXAMPLES_ORCV2EXAMPLES_EXAMPLEMODULES_H
#define LLVM_EXAMPLES_ORCV2EXAMPLES_EXAMPLEMODULES_H

#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/SourceMgr.h"

const llvm::StringRef Add1Example =
    R"(
  define i32 @add1(i32 %x) {
  entry:
    %r = add nsw i32 %x, 1
    ret i32 %r
  }
)";

inline llvm::Error createSMDiagnosticError(llvm::SMDiagnostic &Diag) {
  using namespace llvm;
  std::string Msg;
  {
    raw_string_ostream OS(Msg);
    Diag.print("", OS);
  }
  return make_error<StringError>(std::move(Msg), inconvertibleErrorCode());
}

inline llvm::Expected<llvm::orc::ThreadSafeModule>
parseExampleModule(llvm::StringRef Source, llvm::StringRef Name) {
  using namespace llvm;
  auto Ctx = std::make_unique<LLVMContext>();
  SMDiagnostic Err;
  if (auto M = parseIR(MemoryBufferRef(Source, Name), Err, *Ctx))
    return orc::ThreadSafeModule(std::move(M), std::move(Ctx));

  return createSMDiagnosticError(Err);
}

inline llvm::Expected<llvm::orc::ThreadSafeModule>
parseExampleModuleFromFile(llvm::StringRef FileName) {
  using namespace llvm;
  auto Ctx = std::make_unique<LLVMContext>();
  SMDiagnostic Err;

  if (auto M = parseIRFile(FileName, Err, *Ctx))
    return orc::ThreadSafeModule(std::move(M), std::move(Ctx));

  return createSMDiagnosticError(Err);
}

#endif // LLVM_EXAMPLES_ORCV2EXAMPLES_EXAMPLEMODULES_H

//===--------------- LLJITWithGDBRegistrationListener.cpp -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file shows how to switch LLJIT to use a custom object linking layer (we
// use ObjectLinkingLayer, which is backed by JITLink, as an example).
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/StringMap.h"
#include "llvm/ExecutionEngine/JITEventListener.h"
#include "llvm/ExecutionEngine/JITLink/JITLinkMemoryManager.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/TargetProcess/TargetExecutionUtils.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace llvm::orc;

ExitOnError ExitOnErr;

static cl::opt<std::string>
    EntryPointName("entry", cl::desc("Symbol to call as main entry point"),
                   cl::init("main"));

static cl::list<std::string> InputFiles(cl::Positional, cl::OneOrMore,
                                        cl::desc("input files"));

static cl::list<std::string> InputArgv("args", cl::Positional,
                                       cl::desc("<program arguments>..."),
                                       cl::PositionalEatsArgs);

int main(int argc, char *argv[]) {
  // Initialize LLVM.
  InitLLVM X(argc, argv);

  InitializeAllTargetInfos();
  InitializeAllTargets();
  InitializeAllTargetMCs();
  InitializeAllAsmPrinters();
  InitializeAllAsmParsers();
  InitializeAllDisassemblers();

  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  InitializeNativeTargetAsmParser();
  InitializeNativeTargetDisassembler();
  InitializeAllTargetMCAs();

  cl::ParseCommandLineOptions(argc, argv, "LLJITWithGDBRegistrationListener");
  ExitOnErr.setBanner(std::string(argv[0]) + ": ");

  // Detect the host and set code model to small.
  auto JTMB = ExitOnErr(JITTargetMachineBuilder::detectHost());
  if (!JTMB.getTargetTriple().isOSLinux())
    errs()
        << "Warning: This demo may not work for platforms other than Linux.\n";

  // Create an LLJIT instance and use a custom object linking layer creator to
  // register the GDBRegistrationListener with our RTDyldObjectLinkingLayer.
  auto J =
      ExitOnErr(LLJITBuilder()
                    .setJITTargetMachineBuilder(std::move(JTMB))
                    .setObjectLinkingLayerCreator([&](ExecutionSession &ES,
                                                      const Triple &TT) {
                      auto GetMemMgr = []() {
                        return std::make_unique<SectionMemoryManager>();
                      };
                      auto ObjLinkingLayer =
                          std::make_unique<RTDyldObjectLinkingLayer>(
                              ES, std::move(GetMemMgr));

                      // Register the event listener.
                      ObjLinkingLayer->registerJITEventListener(
                          *JITEventListener::createGDBRegistrationListener());

                      // Make sure the debug info sections aren't stripped.
                      ObjLinkingLayer->setProcessAllSections(true);

                      return ObjLinkingLayer;
                    })
                    .create());

  // Make sure that our process symbols are visible to JIT'd code.
  {
    MangleAndInterner Mangle(J->getExecutionSession(), J->getDataLayout());
    J->getMainJITDylib().addGenerator(
        ExitOnErr(orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
            J->getDataLayout().getGlobalPrefix(),
            [MainName = Mangle("main")](const orc::SymbolStringPtr &Name) {
              return Name != MainName;
            })));
  }

  // Load the input modules.
  for (auto &InputFile : InputFiles) {
    auto Ctx = std::make_unique<LLVMContext>();
    SMDiagnostic Err;
    std::unique_ptr<Module> M = parseIRFile(InputFile, Err, *Ctx);
    if (!M) {
      Err.print(argv[0], errs());
      return 1;
    }

    ExitOnErr(J->addIRModule(ThreadSafeModule(std::move(M), std::move(Ctx))));
  }

  // Look up the entry point, cast it to a C main function pointer, then use
  // runAsMain to call it.
  auto EntryAddr = ExitOnErr(J->lookup(EntryPointName));
  auto EntryFn = EntryAddr.toPtr<int(int, char *[])>();

  return runAsMain(EntryFn, InputArgv, StringRef(InputFiles.front()));
}

