#include <jit.h>
#include <clang_driver.h>

#include <llvm/IR/DiagnosticInfo.h>
#include <llvm/Support/CrashRecoveryContext.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Signals.h>

#if false
#ifndef __sun
#define _POSIX_C_SOURCE 200809L
#else
#define __EXTENSIONS__ 1
#endif

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <replxx.h>

#include <string.h>

int repl_utf8str_codepoint_len( char const* s, int utf8len ) {
	int codepointLen = 0;
	unsigned char m4 = 128 + 64 + 32 + 16;
	unsigned char m3 = 128 + 64 + 32;
	unsigned char m2 = 128 + 64;
	for ( int i = 0; i < utf8len; ++ i, ++ codepointLen ) {
		char c = s[i];
		if ( ( c & m4 ) == m4 ) {
			i += 3;
		} else if ( ( c & m3 ) == m3 ) {
			i += 2;
		} else if ( ( c & m2 ) == m2 ) {
			i += 1;
		}
	}
	return ( codepointLen );
}

int repl_context_len( char const* prefix ) {
	char const wb[] = "";//" \t\n\r\v\f-=+*&^%$#@!,./?<>;:`~'\"[]{}()\\|";
	int i = (int)strlen( prefix ) - 1;
	int cl = 0;
	while ( i >= 0 ) {
		if ( strchr( wb, prefix[i] ) != NULL ) {
			break;
		}
		++ cl;
		-- i;
	}
	return ( cl );
}

#include <argparse/argparse.hpp>
#include <argv_split.h>

void repl_colorHook( char const* str_, ReplxxColor* colors_, int size_, void* ud ) {
	int i = 0;
	for ( ; i < size_; ++ i ) {
		if ( isdigit( str_[i] ) ) {
			colors_[i] = REPLXX_COLOR_BRIGHTMAGENTA;
		}
	}
}

void repl_completionHook(char const* context, replxx_completions* lc, int* contextLen, void* ud) {
  argparse::ArgumentParser * program = (argparse::ArgumentParser*) ud;
  //argv_split parser(program->program_name());
  //parser.parse(context);
  program->parse_args({"/prog", "h"});
  // llvm::SmallVector<repl_command, 20> * stack = (llvm::SmallVector<repl_command, 20>*)ud;
  // repl_command & top_level_commands = stack->back();
	// const char** examples = top_level_commands.get_completions();

	// int utf8ContextLen = repl_context_len( context );
	// int prefixLen = (int)strlen( context ) - utf8ContextLen;
	// *contextLen = repl_utf8str_codepoint_len( context + prefixLen, utf8ContextLen );
	// for (int i = 0;	examples[i] != NULL; ++i) {
	// 	if (strncmp(context + prefixLen, examples[i], utf8ContextLen) == 0) {
	// 		replxx_add_completion(lc, examples[i]);
	// 	}
	// }
}

int repl_main() {
  // repl_data data;
  // repl_command & top_level_commands = data.stack.emplace_back(std::move(repl_command()));
  // top_level_commands.add("help");
  // top_level_commands.add("/history");
  // top_level_commands.add("/unique");
  // top_level_commands.add("/eb");
  // top_level_commands.add("/db");

  argparse::ArgumentParser program("JIT Repl", "1.0", argparse::default_arguments::none);
  program.add_argument("-h", "--help", "help")
          .action([&](const auto & /*unused*/) {
            llvm::outs() << program.help().str();
          })
          .default_value(false)
          .help("shows help message")
          .implicit_value(true)
          .nargs(0);
  program.add_argument("-v", "--version", "version")
          .action([&](const auto & /*unused*/) {
            llvm::outs() << program.version() << "\n";
          })
          .default_value(false)
          .help("prints version information")
          .implicit_value(true)
          .nargs(0);


  // git add subparser
  argparse::ArgumentParser add_command("add");
  add_command.add_description("Add file contents to the index");
  add_command.add_argument("files")
    .help("Files to add content from. Fileglobs (e.g.  *.c) can be given to add all matching files.")
    .remaining();

  // git commit subparser
  argparse::ArgumentParser commit_command("commit");
  commit_command.add_description("Record changes to the repository");
  commit_command.add_argument("-a", "--all")
    .help("Tell the command to automatically stage files that have been modified and deleted.")
    .default_value(false)
    .implicit_value(true);

  commit_command.add_argument("-m", "--message")
    .help("Use the given <msg> as the commit message.");

  // git cat-file subparser
  argparse::ArgumentParser catfile_command("cat-file");
  catfile_command.add_description("Provide content or type and size information for repository objects");
  catfile_command.add_argument("-t")
    .help("Instead of the content, show the object type identified by <object>.");

  catfile_command.add_argument("-p")
    .help("Pretty-print the contents of <object> based on its type.");

  // git submodule subparser
  argparse::ArgumentParser submodule_command("submodule");
  submodule_command.add_description("Initialize, update or inspect submodules");
  argparse::ArgumentParser submodule_update_command("update");
  submodule_update_command.add_description("Update the registered submodules to match what the superproject expects");
  submodule_update_command.add_argument("--init")
    .default_value(false)
    .implicit_value(true);
  submodule_update_command.add_argument("--recursive")
    .default_value(false)
    .implicit_value(true);
  submodule_command.add_subparser(submodule_update_command);

  program.add_subparser(add_command);
  program.add_subparser(commit_command);
  program.add_subparser(catfile_command);
  program.add_subparser(submodule_command);

	Replxx* replxx = replxx_init();
	replxx_install_window_change_handler( replxx );

	int quiet = 0;
	char const* prompt = "\x1b[1;32mreplxx\x1b[0m> ";
	replxx_set_indent_multiline( replxx, true );
  replxx_set_immediate_completion( replxx, false);
	const char* file = "./replxx_history.txt";

	replxx_history_load( replxx, file );
  replxx_set_completion_callback( replxx, repl_completionHook, &program );
  replxx_set_highlighter_callback( replxx, repl_colorHook, replxx );

	printf("starting...\n");

	while (1) {
		char const* result = NULL;
		do {
			result = replxx_input( replxx, prompt );
		} while ( ( result == NULL ) && ( errno == EAGAIN ) );

		if (result == NULL) {
			printf("\n");
			break;
		} else if (!strncmp(result, "/history", 9)) {
			/* Display the current history. */
			int index = 0;
			int size = replxx_history_size( replxx );
			ReplxxHistoryScan* hs = replxx_history_scan_start( replxx );
			ReplxxHistoryEntry he;
			for ( ; replxx_history_scan_next( replxx, hs, &he ) == 0; ++index ) {
				replxx_print( replxx, "%4d: %s\n", index, he.text );
			}
			replxx_history_scan_stop( replxx, hs );
		} else if (!strncmp(result, "/unique", 8)) {
			replxx_set_unique_history( replxx, 1 );
		} else if (!strncmp(result, "/eb", 4)) {
			replxx_enable_bracketed_paste( replxx );
		} else if (!strncmp(result, "/db", 4)) {
			replxx_disable_bracketed_paste( replxx );
		}
		if (*result != '\0') {
			replxx_history_add( replxx, result );
		}
	}
	replxx_history_save( replxx, file );
	printf( "Exiting Replxx\n" );
	replxx_end( replxx );
  return 0;
}

#include <miniedit.h>

#include <pdcurses_curses.h>

void pdcurses_repl() {
    auto screen = newterm(nullptr, stdout, stdin);
    noecho();
    raw();
    nonl();
    keypad(stdscr, TRUE);
    curs_set(0);
    if (has_colors()) {
        short bg = COLOR_BLACK;
        start_color();
        if (use_default_colors() == OK) bg = -1;
        init_pair(1, COLOR_GREEN, bg);
        init_pair(2, COLOR_RED, bg);
        init_pair(3, COLOR_CYAN, bg);
        init_pair(4, COLOR_WHITE, bg);
        init_pair(5, COLOR_MAGENTA, bg);
        init_pair(6, COLOR_BLUE, bg);
        init_pair(7, COLOR_YELLOW, bg);
    }
    while (true) {
      int key = getch();
      
      // llvm::CrashRecoveryContext::Enable();
      // llvm::CrashRecoveryContext CRC;
      // CRC.DumpStackAndCleanupOnFailure = true;
      bool has_key = false;
      // bool has_key_crashed = !CRC.RunSafely([&]() {
        // has_key = ::has_key(key);
      // });
      // if (has_key_crashed) {
      //     printf("has_key() has crashed\n");
      //     printf("unrecognized key: %02X\n", key);
      // } else {
        if (has_key) {
          // llvm::CrashRecoveryContext CRC;
          // CRC.DumpStackAndCleanupOnFailure = true;
          char * key_name;
          // bool key_name_crashed = !CRC.RunSafely([&]() {
            key_name = ::keyname(key);
          // });
          // if (key_name_crashed) {
          //     printf("keyname() has crashed\n");
          //     printf("unrecognized key: %02X\n", key);
          // } else {
              printf("key: %s\n", key_name);
          // }
        } else {
          printf("unrecognized key: %02X\n", key);
        }
      // }
      if (key == 'q') break;
      llvm::sys::PrintStackTraceOnErrorSignal("CRASHED");
    }
    standend();
    refresh();
    curs_set(1);
    endwin();
    delscreen(screen);
}

#endif // false

extern int llvm_orcjit_example_packaged_zip_extract(const char * out_dir);

int main(int argc, char *argv[]) {

    JIT::main_llvm_init main_init(argc, const_cast<const char**>(argv));

    if (!llvm::cl::ParseCommandLineOptions(argc, argv, "JIT"))
      return 1;

    auto dir = std::filesystem::path(TempFile::TempDir()).append("JIT");
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directory(dir);
    }
    auto extracted = dir.append("EXTRACTED");
    if (!std::filesystem::exists(extracted)) {
        std::filesystem::create_directory(extracted);
    }

    llvm_orcjit_example_packaged_zip_extract(extracted.c_str());

    auto LLDB_SERVER_PATH = extracted.append("LLDB_SERVER");

    auto old = ::getenv("LLDB_DEBUGSERVER_PATH");
    ::setenv("LLDB_DEBUGSERVER_PATH", LLDB_SERVER_PATH.c_str(), 1);

    JIT jit;

    // //ClangDriver::call_clang_format({"--help"});
    // //ClangDriver::call_clang_cpp({"--help"});
    // //ClangDriver::call_lldb({"--help"});

    // //ClangDriver::call_clang_format({"--help"});
    // //ClangDriver::call_clang_cpp({"--help"});
    // //ClangDriver::call_lldb({"--help"});

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

    // ClangDriver::call_lldb({"/bin/true"});
    // ClangDriver::call_lldb({"/bin/true"});

    //repl_main();

    //pdcurses_repl();

    auto repl_main_src = ClangDriver::SourceFile("repl_main", "cpp");

    repl_main_src << R"(
#include <lldb/Host/Editline.h>
#include <lldb/Host/FileSystem.h>
#include <lldb/Host/Pipe.h>
#include <lldb/Host/PseudoTerminal.h>
#include <lldb/Utility/Status.h>
#include <lldb/Utility/StringList.h>

using namespace lldb_private;

extern "C" void repl_main() {
  llvm::outs() << "REPL_MAIN from llvm outs\n";
}
)";

    repl_main_src.flush();

    ClangDriver::call_clang_format({ClangDriver::default_format_style, "-i", repl_main_src.path()});
    auto LLVM_18_PATH = extracted.append("LLVM_18.a");

    ClangDriver::call_clang_cpp({
      "-I", "/home/alpine/llvm_18/llvm/include",
      "-I", "/home/alpine/llvm_18/clang/include",
      "-I", "/home/alpine/llvm_18/lldb/include",
      "-I", "/home/alpine/llvm_18/release_BUILD/llvm/include",
      "-I", "/home/alpine/llvm_18/release_BUILD/llvm/tools/lldb/include",
      "-I", "/home/alpine/llvm_18/release_BUILD/LIBEDIT/OUT/include",
      "-O0", "-g3", "-S", "-emit-llvm",
      repl_main_src.path(), "-o", repl_main_src.path_ll()
    });
    jit.add_IR_module(repl_main_src.path_ll());

    // Look up the JIT'd function, cast it to a function pointer, then call it.
    void (*repl_main_src_func)(void) = jit.lookup_as_pointer<void(void)>("repl_main");
    
    repl_main_src_func();

    llvm::outs() << "exiting...\n";

    if (old == nullptr)
      ::unsetenv("LLDB_DEBUGSERVER_PATH");
    else
      ::setenv("LLDB_DEBUGSERVER_PATH", old, 1);

    return 0;
}
