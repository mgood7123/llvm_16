#include <config.h>
#include <compat.h>
#include "llvm/Support/BLAKE3.h"
#include "split_c_archive.h"
#include <string>
#ifdef _WIN32
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

namespace LibZip {
    struct Error {
        zip_error_t error;
        inline Error() {
            printf("LibZip::Error()\n");
            zip_error_init(&error);
        }
        inline virtual ~Error() {
            printf("LibZip::~Error()\n");
            zip_error_fini(&error);
        }

        inline const char * c_str() {
            return zip_error_strerror(&error);
        }
    };

    struct Source {
        zip_source_t * source;
        Error error;

        inline Source() {
            source = nullptr;
        }

        inline Source(void * data, zip_int64_t size) {
            source = nullptr;
            open(data, size);
        }

        inline void open(void * data, zip_int64_t size) {
            close();
            printf("LibZip::Source::open()\n");
            source = zip_source_buffer_create(data, size, 0, &error.error);
            if (source == nullptr) {
                printf("error creating in-memory zip buffer: %s\n", error.c_str());
            }
        }

        inline bool valid() { return source != nullptr; }

        inline void close() {
            if (source != nullptr) {
                printf("LibZip::Source::close()\n");
                zip_source_free(source);
                source = nullptr;
                error = {};
            }
        }

        inline ~Source() {
            close();
        }
    };

    struct Archive {
        Error error;
        zip * archive;
        zip_int64_t count;

        inline Archive() {
            archive = nullptr;
            count = 0;
        }

        inline Archive(Source & src) {
            archive = nullptr;
            open(src);
        }

        inline void open(Source & src) {
            close();
            printf("LibZip::Archive::open()\n");
            archive = zip_open_from_source(src.source, ZIP_CHECKCONS, &error.error);
            if (archive == nullptr) {
                printf("error opening in-memory zip archive: %s\n", error.c_str());
                return;
            }
            zip_source_keep(src.source);
            count = zip_get_num_entries(archive, 0);
        }

        inline bool valid() { return archive != nullptr; }

        inline zip_int64_t entries() { return count; }

        struct Stat {
            struct zip_stat sb;
            bool error = false;
            inline Stat(Archive & archive, zip_int64_t index) {
                stat(archive, index);
            }

            inline void stat(Archive & archive, zip_int64_t index) {
                error = zip_stat_index(archive.archive, index, ZIP_FL_ENC_UTF_8, &sb) < 0;
                if (error) {
                    printf("error stat'ing entry of in-memory zip archive: %s\n", archive.c_str());
                }
            }

            inline bool valid() { return !error; }
            inline bool valid_name() { return sb.valid & ZIP_STAT_NAME; }
            inline bool valid_size() { return sb.valid & ZIP_STAT_SIZE; }
            inline bool valid_compressed_size() { return sb.valid & ZIP_STAT_COMP_SIZE; }
            inline const char * name() { return sb.name; }
            inline zip_int64_t size() { return sb.size; }
            inline zip_int64_t compressed_size() { return sb.comp_size; }
        };

        struct File {
            zip_file_t * file;
            Archive * archive;

            inline File() {
                file = nullptr;
            }

            inline File(Archive & archive, zip_int64_t index, bool compressed) {
                open(archive, index, compressed);
            }

            inline void open(Archive & archive, zip_int64_t index, bool compressed) {
                close();
                printf("LibZip::Archive::File::open()\n");
                file = zip_fopen_index(archive.archive, index, compressed ? ZIP_FL_COMPRESSED : 0);
                if (file == nullptr) {
                    printf("error opening entry of in-memory zip archive: %s\n", c_str());
                }
                this->archive = &archive;
            }

            inline bool valid() { return file != nullptr; }

            inline zip_uint64_t read(void * buffer, zip_int64_t size) {
                auto ret = zip_fread(file, buffer, size);
                if (ret < 0) {
                    printf("error reading entry from in-memory zip archive: %s\n", c_str());
                }
                return ret;
            }

            inline const char * c_str() { return archive->c_str(); }

            inline void close() {
                if (file != nullptr) {
                    printf("LibZip::Archive::File::close()\n");
                    zip_fclose(file);
                    file = nullptr;
                    archive = nullptr;
                }
            }

            inline ~File() {
                close();
            }
        };

        inline const char * c_str() { return zip_strerror(archive); }

        inline Stat stat(zip_int64_t index) { return Stat(*this, index); }

        inline const char * comment(zip_int64_t index) { return zip_file_get_comment(archive, index, nullptr, 0); }

        inline File file(zip_int64_t index, bool compressed) { return File(*this, index, compressed); }

        inline void close() {
            if (archive != nullptr) {
                printf("LibZip::Archive::close()\n");
                zip_discard(archive);
                archive = nullptr;
                error = {};
                count = 0;
            }
        }

        inline ~Archive() {
            close();
        }
    };
}

struct Extraction {
    uint8_t * data = nullptr;
    zip_int64_t size = 0;

    inline void open() {
        close();
        printf("archive size: %" PRId64 " bytes\n", llvm_orcjit_example_packaged_zip_data_total_file_size);
        printf("obtaining archive data...\n");
        data = (uint8_t*) llvm_orcjit_example_packaged_zip_data_total_file_obtain();
        if (data == nullptr) {
            printf("failed to obtain archive data\n");
            size = 0;
            return;
        }
        printf("obtained archive data\n");
        printf("reading archive data...\n");
        size = llvm_orcjit_example_packaged_zip_data_total_file_size;
        printf("obtained archive data\n");
    }

    inline bool valid() { return data != nullptr; }

    inline void close() {
        if (data != nullptr) {
            printf("releasing archive data\n");
            llvm_orcjit_example_packaged_zip_data_total_file_release(data);
            data = nullptr;
            size = 0;
            printf("released archive data\n");
        }
    }

    inline virtual ~Extraction() {
        close();
    }
};

#include "file.h"

int llvm_orcjit_example_packaged_zip_extract(const char * out_dir) {
    Extraction ext;

    ext.open();

    if (!ext.valid()) {
        return 1;
    }

    const int const_buffer_size = 1024*2048;

    {
        auto out_dir_zip = std::string(out_dir) + "/" + "MEM_ARCHIVE.zip";

        File zip_file = File(out_dir_zip.c_str(), "w+");

        if (!zip_file.valid()) {
            printf("error opening disk file for '%s'\n  error: %s\n", out_dir_zip.c_str(), strerror(errno));
        }

        zip_int64_t zip_remaining = ext.size;

        printf("writing contents to '%s' ...\n", out_dir_zip.c_str());
        while (zip_remaining != 0) {
            int buffer_size = zip_remaining < const_buffer_size ? (int) zip_remaining : const_buffer_size;
            if (zip_file.write(ext.data+(ext.size-zip_remaining), buffer_size) < 0) {
                printf("error writing to disk file '%s'\n  error: %s\n", out_dir_zip.c_str(), strerror(errno));
                goto ZIP_WRITTEN;
                break;
            }
            zip_remaining -= buffer_size;
            printf("written %d bytes (%" PRId64 " bytes of %" PRId64 " total bytes) to '%s'\n", buffer_size, (ext.size-zip_remaining), ext.size, out_dir_zip.c_str());
        }
        printf("wrote contents to '%s' ...\n", out_dir_zip.c_str());
    }

    ZIP_WRITTEN:

    LibZip::Source source(ext.data, ext.size);

    if (!source.valid()) {
        return 1;
    }

    LibZip::Archive archive(source);

    if (!archive.valid()) {
        return 1;
    }

    printf("extracting %" PRId64 " file%s...\n", archive.entries(), archive.entries() == 1 ? "" : "s");
    for (zip_int64_t idx = 0; idx < archive.entries(); idx++) {
        LibZip::Archive::Stat stat = archive.stat(idx);
        if (!stat.valid()) {
            printf("file at index %" PRId64 " is not valid, skipping\n", idx);
            continue;
        }
        if (!stat.valid_name()) {
            printf("file at index %" PRId64 " is missing a name, skipping\n", idx);
            continue;
        }
        if (!stat.valid_size()) {
            printf("file at index %" PRId64 " has an unknown size, skipping\n", idx);
            continue;
        }
        
        std::string file_path = std::string(out_dir) + "/" + stat.name();
        std::string file_path_hash = std::string(out_dir) + "/" + stat.name() + ".blake3";
        printf("file path:      %s\n", file_path.c_str());
        printf("file path hash: %s\n", file_path_hash.c_str());
        const char * comment = archive.comment(idx);

        bool is_exe = false;
        bool is_file = false;
        bool is_hash = false;
        bool simple = true;
        if (comment != nullptr) {
            printf("file comment: %s\n", comment);
            if (strcmp(comment, "static archive") == 0) {
                #ifdef _WIN32
                    file_path += ".lib";
                #else
                    file_path += ".a";
                #endif
            } else if (strcmp(comment, "text") == 0) {
                file_path += ".txt";
            } else if (strcmp(comment, "exe") == 0) {
                is_exe = true;
            } else if (strcmp(comment, "blake3") == 0) {
                is_hash = true;
            }
        }

        {
            #ifdef _WIN32
                struct __stat64 sb_;
                int ret = _stat64(file_path.c_str(), &sb_);
            #else
                struct stat sb_;
                int ret = ::stat(file_path.c_str(), &sb_);
            #endif
            if (ret == -1) {
                if (errno != ENOENT) {
                    printf("error stat'ing file '%s': %s\n", file_path.c_str(), strerror(errno));
                }
                goto NO_HASH;
            }

            if (stat.size() != sb_.st_size) {
                if (errno != ENOENT) {
                    printf("size mismatch for file '%s'\n", file_path.c_str());
                }
                goto NO_HASH;
            }

            if (!simple) {
                goto HAS_HASH; // no extract, file matches
            } else if (is_hash) {
                goto NO_HASH; // always extract hash file
            } else {
                goto NEEDS_HASH; // needs hashing, extracts if hash mismatch
            }
        }

        NEEDS_HASH:
        {
            printf("testing file '%s'\n", file_path.c_str());
            bool has_hash = false;
            bool has_matches = false;
            bool has_file_hash = false;
            bool located_hash = false;

            File file = File(file_path_hash.c_str(), "r");
            if (!file.valid()) {
                file = File(file_path.c_str(), "r");
                if (!file.valid()) goto NO_HASH;
            } else {
                has_file_hash = true;
            }

            printf("file exists '%s'\n", file_path.c_str());

            printf("locating hash...\n");
            for (zip_int64_t idx = 0; idx < archive.entries(); idx++) {
                
                LibZip::Archive::Stat stat_ = archive.stat(idx);
                if (!stat_.valid()) {
                    printf("file at index %" PRId64 " is not valid, skipping\n", idx);
                    continue;
                }
                if (!stat_.valid_name()) {
                    printf("file at index %" PRId64 " is missing a name, skipping\n", idx);
                    continue;
                }
                if (!stat_.valid_size()) {
                    printf("file at index %" PRId64 " has an unknown size, skipping\n", idx);
                    continue;
                }
                
                const char * comment = archive.comment(idx);
                if (comment == nullptr) {
                    continue;
                }
                if (strcmp(comment, "blake3") != 0) {
                    continue;
                }

                std::string file_path_ = std::string(out_dir) + "/" + stat_.name();
                printf("potential file path: %s\n", file_path_.c_str());
                if (file_path_hash != file_path_) {
                    continue;
                }

                located_hash = true;
                printf("located hash\n");

                zip_int64_t sz = stat_.size();
                zip_int64_t sz_remaining = stat_.size();

                auto file_ = archive.file(idx, false);
                if (!file_.valid()) {
                    goto ZIP_SRC_CONTINUE2;
                }

                {
                    llvm::BLAKE3Result<LLVM_BLAKE3_OUT_LEN> expected_blake3_hash;

                    int total_written = 0;
                    while (sz_remaining != 0) {
                        int written = 0;
                        int buffer_size = sz_remaining < const_buffer_size ? (int) sz_remaining : const_buffer_size;
                        while(written != buffer_size) {
                            int r = (int) file_.read(&expected_blake3_hash[total_written], buffer_size);
                            if (r < 0) {
                                goto ZIP_SRC_CONTINUE2;
                            }
                            written += r;
                            total_written += written;
                        }
                        sz_remaining -= buffer_size;
                    }
                    {
                        if (has_file_hash) {
                            printf("reading file hash...\n");
                            llvm::BLAKE3Result<LLVM_BLAKE3_OUT_LEN> file_blake3_hash;

                            sz_remaining = sizeof(uint8_t)*LLVM_BLAKE3_OUT_LEN;

                            int total_written = 0;
                            while (sz_remaining != 0) {
                                int written = 0;
                                int buffer_size = sz_remaining < const_buffer_size ? (int) sz_remaining : const_buffer_size;
                                while(written != buffer_size) {
                                    int r = (int) file.read(&file_blake3_hash[total_written], buffer_size);
                                    if (r < 0) {
                                        printf("error reading in-memory zip buffer for '%s'\n  error: %s\n", file_path_.c_str(), strerror(errno));
                                        goto ZIP_SRC_CONTINUE2;
                                    }
                                    written += r;
                                    total_written += written;
                                }
                                sz_remaining -= buffer_size;
                            }

                            printf("expected blake3 hash: ");
                            for (uint8_t byte : expected_blake3_hash) {
                                printf("%02x", byte);
                            }
                            printf("\n");
                            printf("file blake3 hash: ");
                            for (uint8_t byte : file_blake3_hash) {
                                printf("%02x", byte);
                            }
                            printf("\n");
                            
                            if (file_blake3_hash != expected_blake3_hash) {
                                printf("hash mismatch '%s'\n", file_path_hash.c_str());
                            } else {
                                printf("hash match '%s'\n", file_path_hash.c_str());
                                has_hash = true;
                            }
                        }
                        File f2(file_path.c_str(), "r");
                        if (!f2.valid()) {
                            printf("error opening disk file for '%s'\n  error: %s\n", file_path.c_str(), strerror(errno));
                            goto ZIP_SRC_CONTINUE2;
                        } else {
                            printf("writing hash...\n");

                            // Initialize the hasher.
                            llvm::BLAKE3 hasher;

                            // Read input bytes from stdin.
                            char buf[65536];
                            while (1) {
                                ssize_t n = f2.read(buf, sizeof(buf));
                                if (n > 0) {
                                    hasher.update(llvm::StringRef(buf, n));
                                } else if (n == 0) {
                                    break; // end of file
                                } else {
                                    printf("read failed: %s\n", strerror(errno));
                                    f2.close();
                                    goto ZIP_SRC_CONTINUE2;
                                }
                            }

                            // Finalize the hash. Default output length is 32 bytes.
                            auto computed_blake3_hash = hasher.final();
                            f2.close();

                            printf("wrote hash\n");

                            // Print the hash as hexadecimal.
                            printf("computed hash: ");
                            for (uint8_t byte : computed_blake3_hash) {
                                printf("%02x", byte);
                            }
                            printf("\n");
                            if (computed_blake3_hash != expected_blake3_hash) {
                                printf("hash mismatch '%s'\n", file_path.c_str());
                            } else {
                                printf("hash match '%s'\n", file_path.c_str());
                                has_hash = true;
                            }
                        }
                        printf("read hash\n");
                    }
                }
                break;
            }

            ZIP_SRC_CONTINUE2:
            file.close();

            if (has_hash) {
                printf("hash matches for file '%s'\n", file_path.c_str());
                goto HAS_HASH;
            } else {
                if (located_hash) {
                    printf("hash does not match for file '%s'\n", file_path.c_str());
                } else {
                    printf("no hash was computed for file '%s'\n", file_path.c_str());
                }
            }
        }

        NO_HASH:
        {
            printf("extracting '%s' to '%s'\n", stat.name(), file_path.c_str());
            File output(file_path.c_str(), "w+");
            if (!output.valid()) {
                printf("error opening disk file for '%s'\n  error: %s\n", file_path.c_str(), strerror(errno));
                is_exe = false;
                continue;
            }

            zip_int64_t sz = stat.size();
            zip_int64_t sz_remaining = stat.size();
            auto src = archive.file(idx, false);
            if (!src.valid()) {
                is_exe = false;
                goto ZIP_SRC_CONTINUE;
            }

            while (sz_remaining != 0) {
                int written = 0;
                int buffer_size = sz_remaining < const_buffer_size ? (int) sz_remaining : const_buffer_size;
                while(written != buffer_size) {
                    uint8_t buf[buffer_size];
                    int r = (int) src.read(buf, buffer_size);
                    if (r < 0) {
                        is_exe = false;
                        goto ZIP_SRC_CONTINUE;
                    }
                    if (output.write(buf, buffer_size) < 0) {
                        printf("error writing to disk file '%s'\n  error: %s\n", file_path.c_str(), strerror(errno));
                        is_exe = false;
                        goto ZIP_SRC_CONTINUE;
                    }
                    written += r;
                }
                sz_remaining -= buffer_size;
            }

            ZIP_SRC_CONTINUE:
            if (is_exe) {
    #ifdef _WIN32
                if (_chmod(file_path.c_str(), S_IRUSR|S_IWUSR|S_IXUSR) < 0) {
                    printf("error applying executable permission for '%s'\n  error: %s\n", file_path.c_str(), strerror(errno));
                }
    #else
                if (chmod(file_path.c_str(), S_IRUSR|S_IWUSR|S_IXUSR) < 0) {
                    printf("error applying executable permission for '%s'\n  error: %s\n", file_path.c_str(), strerror(errno));
                }
    #endif
            }
        }
        HAS_HASH:
        continue;
    }
    printf("extracted files\n");
    return 0;
}
