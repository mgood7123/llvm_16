#include <clang/Config/config.h>
#include <llvm/Support/LLVMDriver.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include "llvm/Support/raw_ostream.h"

#include <tmpfile.h>
#include <filesystem>

extern int clang_main(int Argc, char **Argv, const llvm::ToolContext &ToolContext);
extern int format_main(int argc, const char **argv);
extern int lldb_main(int argc, const char **argv);

struct ClangDriver {

    static const char * default_format_style;

    static inline int call_clang_c() {
        return call_clang_c({});
    }

    static inline int call_clang_cpp() {
        return call_clang_cpp({});
    }

    static inline int call_clang_preprocessor() {
        return call_clang_preprocessor({});
    }

    static inline int call_clang_cl() {
        return call_clang_cl({});
    }

    static inline int call_clang_format() {
        return call_clang_format({});
    }

    static inline int call_clang_c(const llvm::SmallVector<const char*> & args) {
        llvm::SmallVector<const char*> copy = args;

        // path does not need to exist
        //
        copy.insert(copy.begin(), "/llvm/mc/jit/clang");
        copy.push_back("-fintegrated-as");
        copy.push_back("-fintegrated-cc1");
        copy.push_back("-fintegrated-objemitter");
        llvm::cl::ResetAllOptionOccurrences();
        return clang_main(copy.size(), const_cast<char**>(copy.data()), {copy[0], copy[0], false});
    }
    static inline int call_clang_cpp(const llvm::SmallVector<const char*> & args) {
        llvm::SmallVector<const char*> copy = args;

        // path does not need to exist
        //
        copy.insert(copy.begin(), "/llvm/mc/jit/clang++");
        copy.push_back("-fintegrated-as");
        copy.push_back("-fintegrated-cc1");
        copy.push_back("-fintegrated-objemitter");
        llvm::cl::ResetAllOptionOccurrences();
        return clang_main(copy.size(), const_cast<char**>(copy.data()), {copy[0], copy[0], false});
    }
    static inline int call_clang_preprocessor(const llvm::SmallVector<const char*> & args) {
        llvm::SmallVector<const char*> copy = args;

        // path does not need to exist
        //
        copy.insert(copy.begin(), "/llvm/mc/jit/clang-cpp");
        copy.push_back("-fintegrated-as");
        copy.push_back("-fintegrated-cc1");
        copy.push_back("-fintegrated-objemitter");
        llvm::cl::ResetAllOptionOccurrences();
        return clang_main(copy.size(), const_cast<char**>(copy.data()), {copy[0], copy[0], false});
    }
    static inline int call_clang_cl(const llvm::SmallVector<const char*> & args) {
        llvm::SmallVector<const char*> copy = args;

        // path does not need to exist
        //
        copy.insert(copy.begin(), "/llvm/mc/jit/clang-cl");
        copy.push_back("-fintegrated-as");
        copy.push_back("-fintegrated-cc1");
        copy.push_back("-fintegrated-objemitter");
        llvm::cl::ResetAllOptionOccurrences();
        return clang_main(copy.size(), const_cast<char**>(copy.data()), {copy[0], copy[0], false});
    }
    static inline int call_clang_format(const llvm::SmallVector<const char*> & args) {
        llvm::SmallVector<const char*> copy = args;

        // path does not need to exist
        //
        copy.insert(copy.begin(), "/llvm/mc/jit/clang-format");
        llvm::cl::ResetAllOptionOccurrences();
        return format_main(copy.size(), copy.data());
    }
    static inline int call_lldb(const llvm::SmallVector<const char*> & args) {
        llvm::SmallVector<const char*> copy = args;

        // path does not need to exist
        //
        copy.insert(copy.begin(), "/llvm/mc/jit/clang-format");
        llvm::cl::ResetAllOptionOccurrences();
        return lldb_main(copy.size(), copy.data());
    }
    
    // we cannot `: raw_fd_ostream` since that requires pre-setup of our file
    // the best we can do is unique_ptr it and template operator << to forward all supported <<
    class SourceFile {
        TempFileFD file;
        llvm::SmallString<256> file_path_ll;
        std::unique_ptr<llvm::raw_fd_ostream> stream;

        public:

        inline SourceFile(const char * file_name, const char * extension) {
            auto dir = std::filesystem::path(TempFile::TempDir()).append("JIT");
            if (!std::filesystem::exists(dir)) {
                std::filesystem::create_directory(dir);
            }
            std::string f = (llvm::Twine(file_name) + "-").str();
            std::string e = (llvm::Twine(".") + extension).str();
            file.construct(dir.c_str(), f.c_str(), e.c_str());
            int handle = file.get_handle();
            if (file.is_valid()) {
                file_path_ll = (llvm::Twine(file.get_path().c_str()) + ".ll").str();
                stream = std::make_unique<llvm::raw_fd_ostream>(handle, false, false);
            }
        }

        template <typename T>
        inline SourceFile & operator << (const T & data) {
            *stream << data;
            return *this;
        }

        template <typename T>
        inline SourceFile & operator << (T & data) {
            *stream << data;
            return *this;
        }

        template <typename T>
        inline SourceFile & operator << (const T && data) {
            *stream << std::move(data);
            return *this;
        }

        template <typename T>
        inline SourceFile & operator << (T && data) {
            *stream << std::move(data);
            return *this;
        }

        inline void flush() { stream->flush(); }

        inline bool is_valid() { return file.is_valid(); }

        inline const char * path() { return file.get_path().c_str(); }
        inline const char * path_ll() { return file_path_ll.c_str(); }

        inline ~SourceFile() { stream->flush(); }
    };
};