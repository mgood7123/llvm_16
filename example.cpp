#include <llvm/ADT/StringRef.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ExecutionEngine/JITEventListener.h>
#include <llvm/ExecutionEngine/JITLink/JITLinkMemoryManager.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/Orc/TargetProcess/TargetExecutionUtils.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

// additional info for --version
#include <llvm/MC/TargetRegistry.h>
#include <llvm/TargetParser/Host.h>

llvm::ExitOnError ExitOnErr;

struct ModuleHolder {
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;

    ModuleHolder(const char * name) {
        context = std::make_unique<llvm::LLVMContext>();
        module = std::make_unique<llvm::Module>(name, *context);
    }

    llvm::orc::ThreadSafeModule create() {
        return std::move(llvm::orc::ThreadSafeModule(std::move(module), std::move(context)));
    }
};

int main(int argc, char *argv[]) {
    // Initialize LLVM.
    llvm::InitLLVM X(argc, argv);

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllDisassemblers();

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetDisassembler();
    llvm::InitializeAllTargetMCAs();

    // Register the Target and CPU printer for --version.
    llvm::cl::AddExtraVersionPrinter(llvm::sys::printDefaultTargetAndDetectedCPU);
    // Register the target printer for --version.
    llvm::cl::AddExtraVersionPrinter(llvm::TargetRegistry::printRegisteredTargetsForVersion);

    llvm::cl::ParseCommandLineOptions(argc, argv, "LLJITWithInitializers");
    ExitOnErr.setBanner(std::string(argv[0]) + ": ");

    // Detect the host and set code model to small.
    auto JTMB = ExitOnErr(llvm::orc::JITTargetMachineBuilder::detectHost());
    if (!JTMB.getTargetTriple().isOSLinux())
        llvm::errs() << "Warning: This demo may not work for platforms other than Linux.\n";

    // Create an LLJIT instance and use a custom object linking layer creator to
    // register the GDBRegistrationListener with our RTDyldObjectLinkingLayer.
    auto J =
        ExitOnErr(llvm::orc::LLJITBuilder()
                        .setJITTargetMachineBuilder(std::move(JTMB))
                        .setObjectLinkingLayerCreator([&](llvm::orc::ExecutionSession &ES,
                                                        const llvm::Triple &TT) {
                        auto GetMemMgr = []() {
                            return std::make_unique<llvm::SectionMemoryManager>();
                        };
                        auto ObjLinkingLayer =
                            std::make_unique<llvm::orc::RTDyldObjectLinkingLayer>(
                                ES, std::move(GetMemMgr));

                        // Register the event listener.
                        ObjLinkingLayer->registerJITEventListener(
                            *llvm::JITEventListener::createGDBRegistrationListener());

                        // Make sure the debug info sections aren't stripped.
                        ObjLinkingLayer->setProcessAllSections(true);

                        return ObjLinkingLayer;
                        })
                        .create());

    /*

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

        ExitOnErr(
        J->addIRModule(
            ThreadSafeModule(std::move(M), std::move(Ctx))
        )
        );
    }
    */

    // Load the module.

    // ModuleHolder mb("example.c");
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;

    context = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>("example.c", *context);

    const char * func_name = "main2";

    // add debug info
    llvm::DIBuilder debug(*module);
    auto debug_file = debug.createFile("example.c", "/home/DATA/git/Gecko/HTML/qparse/ManagedObject/llvm_mcjit");
    auto cu = debug.createCompileUnit(
        llvm::dwarf::DW_LANG_C99, // Lang
        debug_file,               // File
        "LLVM", // Producer
        false, // isOptimized
        "", // Flags
        0, // RV
        llvm::StringRef(), // SplitName
        llvm::DICompileUnit::DebugEmissionKind::FullDebug, // Kind
        0, // DWOld
        false, // DebugSplitInlining
        false, // DebugInfoForProfiling
        llvm::DICompileUnit::DebugNameTableKind::None, // NameTableKind
        false // RangesBaseAddress
    );
    module->addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);
    // mac os only supports dwarf2.
    module->addModuleFlag(llvm::Module::Warning, "Dwarf Version", 2);

    auto func_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), /*not vararg*/false);
    auto func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, func_name, *module);

    // debug info - func_type
    auto debug_int_type = debug.createBasicType("int", 32, llvm::dwarf::DW_ATE_signed);
    llvm::SmallVector<llvm::Metadata*, 1> debug_func_array;
    debug_func_array.push_back(debug_int_type);
    auto debug_func_type_array = debug.getOrCreateTypeArray(debug_func_array);
    auto debug_func_type = debug.createSubroutineType(debug_func_type_array);

    // debug info - func
    llvm::DIScope* debug_scope = (llvm::DIScope*) debug_file;
    unsigned line_no = 2;
    unsigned scope_line = 1;
    auto debug_func_subprogram = debug.createFunction(
        debug_file, func_name, llvm::StringRef(), debug_file,
        line_no, debug_func_type, scope_line,
        llvm::DINode::FlagZero, llvm::DISubprogram::SPFlagDefinition
    );
    func->setSubprogram(debug_func_subprogram);

    // function body
    {
        auto basic_block = llvm::BasicBlock::Create(*context, "EntryBlock", func);
        auto value_two = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 2);
        auto r = llvm::ReturnInst::Create(*context, value_two);
        r->setDebugLoc(llvm::DebugLoc(llvm::DILocation::get(*context, 1, 1, debug_func_subprogram)));
        r->insertInto(basic_block, basic_block->end());
    }

    debug.finalize();
    llvm::errs() << "printing module...\n";
    module->print(llvm::errs(), nullptr);
    llvm::errs() << "printed module...\n\n";

    // auto m = mb.create();
    auto m = std::move(llvm::orc::ThreadSafeModule(std::move(module), std::move(context)));
    ExitOnErr(J->addIRModule(std::move(m)));

    // auto Ctx = std::make_unique<llvm::LLVMContext>();
    // llvm::SMDiagnostic Err;
    // std::unique_ptr<llvm::Module> M = llvm::parseIRFile("./example.clang.ll", Err, *Ctx);
    // if (!M) {
    //     Err.print(argv[0], llvm::errs());
    //         return 1;
    // }

    // ExitOnErr(
    //     J->addIRModule(
    //         llvm::orc::ThreadSafeModule(std::move(M), std::move(Ctx))
    //     )
    // );

    // Look up the JIT'd function, cast it to a function pointer, then call it.
    auto main_addr = ExitOnErr(J->lookup(func_name));
    int (*main_func)(void) = main_addr.toPtr<int(void)>();

    int res = main_func();
    llvm::outs() << func_name << "() = " << res << "\n";

    // int32_t InitializersRunFlag = 0;
    // int32_t DeinitializersRunFlag = 0;

    // ExitOnErr(J->getMainJITDylib().define(
    //     llvm::orc::absoluteSymbols({{J->mangleAndIntern("InitializersRunFlag"),
    //                       {llvm::orc::ExecutorAddr::fromPtr(&InitializersRunFlag),
    //                        llvm::JITSymbolFlags::Exported}},
    //                      {J->mangleAndIntern("DeinitializersRunFlag"),
    //                       {llvm::orc::ExecutorAddr::fromPtr(&DeinitializersRunFlag),
    //                        llvm::JITSymbolFlags::Exported}}})));

    // Run static initializers.
    // ExitOnErr(J->initialize(J->getMainJITDylib()));

    // Run deinitializers.
    // ExitOnErr(J->deinitialize(J->getMainJITDylib()));

    // llvm::outs() << "InitializerRanFlag = " << InitializersRunFlag << "\n"
    //        << "DeinitializersRunFlag = " << DeinitializersRunFlag << "\n";

    llvm::llvm_shutdown();

    return 0;
}
