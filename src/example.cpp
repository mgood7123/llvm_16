#include "jit.h"
#include <clang_driver.h>

int main(int argc, char *argv[]) {

    JIT::main_llvm_init main_init(argc, const_cast<const char**>(argv));

    if (!llvm::cl::ParseCommandLineOptions(argc, argv, "JIT"))
      return 1;

    JIT jit;

    ClangDriver::call_clang_format({"--help"});
    ClangDriver::call_clang_cpp({"--help"});

    auto hello = ClangDriver::SourceFile("hello", "cpp");

    const char * func_name = "main2";

    hello << "extern \"C\" int " << func_name << "() {";
    hello << "return 42;";
    hello << "}";

    hello.flush();

    ClangDriver::call_clang_format({ClangDriver::default_format_style, "-i", hello.path()});
    ClangDriver::call_clang_cpp({"-S", "-O0", "-g3", "-emit-llvm", hello.path(), "-o", hello.path_ll()});
    jit.add_IR_module(hello.path_ll());

    // Look up the JIT'd function, cast it to a function pointer, then call it.
    int (*main_func)(void) = jit.lookup_as_pointer<int(void)>("main2");

    int res = main_func();
    llvm::outs() << func_name << "() = " << res << "\n";

    return 0;
}
