#include <string>
#include <vector>

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

#include <zip.h>

#include "zpack_cpp.h"

#include "llvm/Support/BLAKE3.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file.h"

#include <archive.h>
#include <archive_entry.h>

static void
progress_callback(zip_t *archive, double percentage, void *ud) {
    printf("%.3f%% done\r", percentage * 100);
}

int main(int argc, char ** argv) {
    printf("PACKAGER INVOKED()\n");

    printf("argc: %d\n", argc);
    std::vector<std::pair<std::string, std::string>> static_libs;
    std::vector<std::string> sys_libs;
    std::vector<std::pair<std::string, std::string>> executables;
    bool is_static = false;
    bool is_sys = false;
    bool is_exe = false;
    bool is_link_libs = false;
    bool is_inc_libs = false;
    std::pair<std::string, std::string> p;

    std::string output_file_dir;
    std::string output_file_name;
    std::string output_file_suffix;
    std::string data_label;
    std::string cmake_file;
    std::vector<std::string> cmake_link_libs;
    std::vector<std::string> cmake_link_includes;

    std::vector<std::string> buffers;
    std::vector<llvm::BLAKE3Result<LLVM_BLAKE3_OUT_LEN>> buffer_hashes;

    #if false
    printf("\n\n");
    for (int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n\n");
    #endif

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "STATIC_LIBS") == 0) {
            is_static = true;
            is_sys = false;
            is_exe = false;
            is_link_libs = false;
            is_inc_libs = false;
            continue;
        } else if (strcmp(argv[i], "SYS_LIBS") == 0) {
            is_static = false;
            is_sys = true;
            is_exe = false;
            is_link_libs = false;
            is_inc_libs = false;
            continue;
        } else if (strcmp(argv[i], "EXECUTABLES") == 0) {
            is_static = false;
            is_sys = false;
            is_exe = true;
            is_link_libs = false;
            is_inc_libs = false;
            continue;
        } else if (strcmp(argv[i], "CMAKE_LINK_LIBS") == 0) {
            is_static = false;
            is_sys = false;
            is_exe = false;
            is_link_libs = true;
            is_inc_libs = false;
            continue;
        } else if (strcmp(argv[i], "CMAKE_LINK_INCLUDES") == 0) {
            is_static = false;
            is_sys = false;
            is_exe = false;
            is_link_libs = false;
            is_inc_libs = true;
            continue;
        } else if (strcmp(argv[i], "OUTPUT_FILE_DIRECTORY") == 0) {
            i++;
            if (i < argc) {
                output_file_dir = argv[i];
            } else {
                printf("error: OUTPUT_FILE_DIRECTORY must be followed by a PATH\n");
                return 1;
            }
            continue;
        } else if (strcmp(argv[i], "OUTPUT_FILE_NAME") == 0) {
            i++;
            if (i < argc) {
                output_file_name = argv[i];
            } else {
                printf("error: OUTPUT_FILE_NAME must be followed by a PATH\n");
                return 1;
            }
            continue;
        } else if (strcmp(argv[i], "OUTPUT_FILE_SUFFIX") == 0) {
            i++;
            if (i < argc) {
                output_file_suffix = argv[i];
            } else {
                printf("error: OUTPUT_FILE_SUFFIX must be followed by a PATH\n");
                return 1;
            }
            continue;
        } else if (strcmp(argv[i], "OUTPUT_DATA_LABEL") == 0) {
            i++;
            if (i < argc) {
                data_label = argv[i];
            } else {
                printf("error: OUTPUT_DATA_LABEL must be followed by a string\n");
                return 1;
            }
            continue;
        } else if (strcmp(argv[i], "OUTPUT_CMAKE") == 0) {
            i++;
            if (i < argc) {
                cmake_file = argv[i];
            } else {
                printf("error: OUTPUT_CMAKE must be followed by a string\n");
                return 1;
            }
            continue;
        }

        if (is_static) {
            i++;
            if (i < argc) {
                static_libs.emplace_back(argv[i-1], argv[i]);
            } else {
                printf("error: STATIC_LIBS must be followed by an array of NAME, PATH\n");
                return 1;
            }
        } else if (is_sys) {
            sys_libs.emplace_back(argv[i]);
        } else if (is_exe) {
            i++;
            if (i < argc) {
                executables.emplace_back(argv[i-1], argv[i]);
            } else {
                printf("error: EXECUTABLES must be followed by an array of NAME, PATH\n");
            }
        } else if (is_link_libs) {
            cmake_link_libs.emplace_back(argv[i]);
        } else if (is_inc_libs) {
            cmake_link_includes.emplace_back(argv[i]);
        } else {
            printf("ignoring argv[%d] = %s\n", i, argv[i]);
        }
    }

    /*

    for (int i = 0; i < static_libs.size(); i++) {
        printf("static_libs[%d] = { name = '%s', path = '%s' }\n", i, static_libs[i].first.c_str(), static_libs[i].second.c_str());
    }

    for (int i = 0; i < sys_libs.size(); i++) {
        printf("sys_libs[%d] = %s\n", i, sys_libs[i].c_str());
    }

    for (int i = 0; i < executables.size(); i++) {
        printf("executables[%d] = { name = '%s', path = '%s' }\n", i, executables[i].first.c_str(), executables[i].second.c_str());
    }

    */

    // write hash files first
    // for (int i = 0; i < static_libs.size(); i++) {
    //     // BLAKE3
    //     {
    //         printf("writing hash...\n");
    //         File f(static_libs[i].second.c_str(), "r");
    //         if (!f.valid()) {
    //             printf("error opening disk zip file: %s\n", strerror(errno));
    //             return 1;
    //         }

    //         // Initialize the hasher.
    //         llvm::BLAKE3 hasher;

    //         // Read input bytes from stdin.
    //         char buf[65536];
    //         while (1) {
    //             ssize_t n = f.read(buf, sizeof(buf));
    //             if (n > 0) {
    //                 hasher.update(llvm::StringRef(buf, n));
    //             } else if (n == 0) {
    //                 break; // end of file
    //             } else {
    //                 printf("read failed: %s\n", strerror(errno));
    //                 return 1;
    //             }
    //         }

    //         // Finalize the hash. Default output length is 32 bytes.
    //         buffer_hashes.emplace_back(hasher.final());
    //         llvm::BLAKE3Result<LLVM_BLAKE3_OUT_LEN> & output = buffer_hashes.back();

    //         printf("wrote hash\n");

    //         // Print the hash as hexadecimal.
    //         printf("hash: ");
    //         for (uint8_t byte : output) {
    //             printf("%02x", byte);
    //         }
    //         printf("\n");

    //         buffers.emplace_back(static_libs[i].first + ".blake3");
    //         std::string & HASH = buffers.back();

    //         stream.write(writer, HASH.c_str(), output.data(), output.size(), ZPACK_COMPRESSION_LZ4, 1);
    //         if (stream.error_msg != nullptr) {
    //             printf("%s\n", stream.error_msg);
    //             return 1;
    //         }
    //     }
    // }

    // for (int i = 0; i < executables.size(); i++) {
    //     // BLAKE3
    //     {
    //         printf("writing hash...\n");
    //         File f(executables[i].second.c_str(), "r");
    //         if (!f.valid()) {
    //             printf("error opening disk zip file: %s\n", strerror(errno));
    //             return 1;
    //         }

    //         // Initialize the hasher.
    //         llvm::BLAKE3 hasher;

    //         // Read input bytes from stdin.
    //         char buf[65536];
    //         while (1) {
    //             ssize_t n = f.read(buf, sizeof(buf));
    //             if (n > 0) {
    //                 hasher.update(llvm::StringRef(buf, n));
    //             } else if (n == 0) {
    //                 break; // end of file
    //             } else {
    //                 printf("read failed: %s\n", strerror(errno));
    //                 return 1;
    //             }
    //         }

    //         // Finalize the hash. Default output length is 32 bytes.
    //         buffer_hashes.emplace_back(hasher.final());
    //         llvm::BLAKE3Result<LLVM_BLAKE3_OUT_LEN> & output = buffer_hashes.back();

    //         printf("wrote hash\n");

    //         // Print the hash as hexadecimal.
    //         printf("hash: ");
    //         for (uint8_t byte : output) {
    //             printf("%02x", byte);
    //         }
    //         printf("\n");

    //         buffers.emplace_back(executables[i].first + ".blake3");
    //         std::string & HASH = buffers.back();

    //         stream.write(writer, HASH.c_str(), output.data(), output.size(), ZPACK_COMPRESSION_LZ4, 1);
    //         if (stream.error_msg != nullptr) {
    //             printf("%s\n", stream.error_msg);
    //             return 1;
    //         }
    //     }
    // }

    if (sys_libs.size() != 0) {
        buffers.emplace_back("");
        std::string & libs = buffers.back();

        for (int i = 0; i < sys_libs.size(); i++) {
            libs += "-l" + sys_libs[i];
            if (i+1 < sys_libs.size()) libs += " ";
        }
        
        // BLAKE3
        {
            printf("writing hash...\n");

            // Initialize the hasher.
            llvm::BLAKE3 hasher;

            // Read input bytes from stdin.
            hasher.update(llvm::StringRef(libs.c_str(), libs.size()));

            // Finalize the hash. Default output length is 32 bytes.
            buffer_hashes.emplace_back(hasher.final());
            llvm::BLAKE3Result<LLVM_BLAKE3_OUT_LEN> & output = buffer_hashes.back();

            printf("wrote hash\n");

            // Print the hash as hexadecimal.
            printf("hash: ");
            for (uint8_t byte : output) {
                printf("%02x", byte);
            }
            printf("\n");

            File file("/tmp/SYSTEM_LIBRARIES.blake3", "w+");
            if (file.valid()) {
                file.write(output.data(), output.size());
                file.close();
            }

            buffers.emplace_back("SYSTEM_LIBRARIES.blake3");
            std::string & HASH = buffers.back();

            {
                #define CHECK_ARCHIVE(arch, x) { \
                    auto tmp = x; \
                    const char * error = archive_error_string(arch); \
                    if (error != nullptr) { \
                        printf("%s returned error: %s\n", #x, error); \
                    } else { \
                        std::string tmp2 = std::to_string(tmp); \
                        printf("%s returned %s\n", #x, tmp2.c_str()); \
                    } \
                }

/*-
 * To create an archive:
 *   1) Ask archive_write_new for an archive writer object.
 *   2) Set any global properties.  In particular, you should set
 *      the compression and format to use.
 *   3) Call archive_write_open to open the file (most people
 *       will use archive_write_open_file or archive_write_open_fd,
 *       which provide convenient canned I/O callbacks for you).
 *   4) For each entry:
 *      - construct an appropriate struct archive_entry structure
 *      - archive_write_header to write the header
 *      - archive_write_data to write the entry data
 *   5) archive_write_close to close the output
 *   6) archive_write_free to cleanup the writer and release resources
 */

                uint8_t arch_buffer[500];
                memset(arch_buffer, 0, 500);
                size_t arch_size;

                struct archive * arch = archive_write_new();
                CHECK_ARCHIVE(arch, archive_write_set_format_7zip(arch));
                CHECK_ARCHIVE(arch, archive_write_add_filter_lzma(arch));

                CHECK_ARCHIVE(arch, archive_write_open_memory(arch, arch_buffer, 500, &arch_size));

                // CHECK_ARCHIVE(arch, archive_write_open_filename(arch, "/tmp/archive.tar.xz"));

                struct archive_entry * file = archive_entry_new2(arch);
                archive_entry_set_filetype(file, AE_IFREG);
                archive_entry_set_size(file, output.size());
                archive_entry_copy_pathname(file, HASH.c_str());
                CHECK_ARCHIVE(arch, archive_write_header(arch, file));
                CHECK_ARCHIVE(arch, archive_write_data(arch, output.data(), output.size()));

                archive_entry_free(file);

                CHECK_ARCHIVE(arch, archive_write_close(arch));
                archive_write_free(arch);

/*-
 * Basic outline for reading an archive:
 *   1) Ask archive_read_new for an archive reader object.
 *   2) Update any global properties as appropriate.
 *      In particular, you'll certainly want to call appropriate
 *      archive_read_support_XXX functions.
 *   3) Call archive_read_open_XXX to open the archive
 *   4) Repeatedly call archive_read_next_header to get information about
 *      successive archive entries.  Call archive_read_data to extract
 *      data for entries of interest.
 *   5) Call archive_read_free to end processing.
 */

                printf("archive size: %zu\n", arch_size);
                arch = archive_read_new();
                CHECK_ARCHIVE(arch, archive_read_support_format_all(arch));
                CHECK_ARCHIVE(arch, archive_read_support_filter_lzma(arch));
                CHECK_ARCHIVE(arch, archive_read_open_memory(arch, arch_buffer, arch_size));

                archive_entry_clear(file);
                CHECK_ARCHIVE(arch, archive_read_next_header(arch, &file));
                if (file != nullptr) {
                    printf("file type: %s\n", archive_entry_filetype(file) == AE_IFREG ? "regular file" : "unknown");
                    printf("file size is set: %s\n", archive_entry_size_is_set(file) ? "yes" : "no");
                    if (archive_entry_size_is_set(file)) {
                        printf("file size: %ld\n", archive_entry_size(file));
                    }
                    printf("file path: %s\n", archive_entry_pathname(file));
                }

                CHECK_ARCHIVE(arch, archive_read_close(arch));
                archive_read_free(arch);
            }



            ZPack::HeapWriter writer;
            if (writer.error_msg != nullptr) {
                printf("%s\n", writer.error_msg);
                return 1;
            }
            ZPack::Stream stream;
            if (stream.error_msg != nullptr) {
                printf("%s\n", writer.error_msg);
                return 1;
            }

            stream.write(writer, HASH.c_str(), output.data(), output.size(), ZPACK_COMPRESSION_LZ4, 1);
            if (stream.error_msg != nullptr) {
                printf("%s\n", stream.error_msg);
                return 1;
            }
            if (writer.error_msg != nullptr) {
                printf("%s\n", writer.error_msg);
                return 1;
            }

            // finalize archive
            zpack_u8 * out = nullptr;
            size_t out_size = 0;
            writer.finalize(&out, &out_size);
            if (writer.error_msg != nullptr) {
                printf("%s\n", writer.error_msg);
                return 1;
            }

            printf("archive buffer: %p\n", out);
            printf("archive size: %zu\n", out_size);

            {
                ZPack::HeapReader reader;
                if (reader.error_msg != nullptr) {
                    printf("%s\n", reader.error_msg);
                    return 1;
                }
                reader.open(out, out_size);
                if (reader.error_msg != nullptr) {
                    printf("%s\n", reader.error_msg);
                    return 1;
                }
                reader.print();
                if (reader.error_msg != nullptr) {
                    printf("%s\n", reader.error_msg);
                    return 1;
                }
                return 1;
            }
        }
    }

    // create a in-memory zip-archive
    zip_source_t * zip_memory;
    zip * zip_archive;

    auto METHOD = ZIP_CM_DEFLATE;
    int COMPRESSION = 7;
    // auto METHOD = ZIP_CM_STORE;
    // int COMPRESSION = 1;

    int zip_error_code = 0;

    {
        zip_error_t error;
        zip_error_init(&error);
        zip_memory = zip_source_buffer_create(0, 0, 0, &error);
        if (zip_memory == nullptr) {
            printf("error creating in-memory zip buffer: %s\n", zip_error_strerror(&error));
            zip_error_fini(&error);
            return 1;
        }
        zip_error_fini(&error);

        // open zip archive from memory
        zip_error_init(&error);
        zip_archive = zip_open_from_source(zip_memory, ZIP_TRUNCATE, &error);
        if (zip_archive == nullptr) {
            printf("error opening in-memory zip archive: %s\n", zip_error_strerror(&error));
            zip_error_code = 1;
            zip_error_fini(&error);
            goto ZIP_MEM_EXIT;
        }
        zip_error_fini(&error);

        // we dont need error anymore
        zip_source_keep(zip_memory);

        zip_register_progress_callback_with_state(zip_archive, 0, progress_callback, nullptr, nullptr);
    }

    // modify zip archive

    // compression level 8 and higher takes longer and doesnt affect the resulting file size much

    for (int i = 0; i < static_libs.size(); i++) {
        printf("adding static archive (name + data) : name = '%s', path = '%s'\n", static_libs[i].first.c_str(), static_libs[i].second.c_str());
        zip_source_t * zip_src = zip_source_file(zip_archive, static_libs[i].second.c_str(), 0, ZIP_LENGTH_TO_END);
        if (zip_src == nullptr) {
            printf("error reading static archive (name + data) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            goto ZIP_ARCHIVE_EXIT;
        }
        zip_int64_t index = zip_file_add(zip_archive, static_libs[i].first.c_str(), zip_src, ZIP_FL_ENC_UTF_8);
        if (index < 0) {
            printf("error adding static archive (name + data) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            zip_source_free(zip_src);
            goto ZIP_ARCHIVE_EXIT;
        }
        // zip_source_free(3) should not be called on a source after it was used
        //  successfully in a zip_file_add or zip_file_replace call.
        if (zip_set_file_compression(zip_archive, index, METHOD, COMPRESSION) < 0) {
            printf("error setting compression settings for static archive (name + data) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            goto ZIP_ARCHIVE_EXIT;
        }
        if (zip_file_set_comment(zip_archive, index, "static archive", strlen("static archive"), ZIP_FL_ENC_UTF_8) < 0) {
            printf("error setting comment for static archive (name + data) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            goto ZIP_ARCHIVE_EXIT;
        }

        // BLAKE3
        {
            printf("writing hash...\n");
            FILE* f = fopen(static_libs[i].second.c_str(), "r");
            if (f == nullptr) {
                printf("error opening disk zip file: %s\n", strerror(errno));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }

            // Initialize the hasher.
            llvm::BLAKE3 hasher;

            // Read input bytes from stdin.
            char buf[65536];
            while (1) {
                ssize_t n = fread(buf, 1, sizeof(buf), f);
                if (n > 0) {
                    hasher.update(llvm::StringRef(buf, n));
                } else if (n == 0) {
                    break; // end of file
                } else {
                    printf("read failed: %s\n", strerror(errno));
                    fclose(f);
                    goto ZIP_ARCHIVE_EXIT;
                }
            }

            fclose(f);

            // Finalize the hash. Default output length is 32 bytes.
            buffer_hashes.emplace_back(hasher.final());
            llvm::BLAKE3Result<LLVM_BLAKE3_OUT_LEN> & output = buffer_hashes.back();

            printf("wrote hash\n");

            // Print the hash as hexadecimal.
            printf("hash: ");
            for (uint8_t byte : output) {
                printf("%02x", byte);
            }
            printf("\n");

            buffers.emplace_back(static_libs[i].first + ".blake3");
            std::string & HASH = buffers.back();

            zip_source_t * zip_src = zip_source_buffer(zip_archive, output.data(), output.size(), 0);
            if (zip_src == nullptr) {
                printf("error reading static archive (name + data) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }
            zip_int64_t index = zip_file_add(zip_archive, HASH.c_str(), zip_src, ZIP_FL_ENC_UTF_8);
            if (index < 0) {
                printf("error adding static archive (name + data) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                zip_source_free(zip_src);
                goto ZIP_ARCHIVE_EXIT;
            }
            // zip_source_free(3) should not be called on a source after it was used
            //  successfully in a zip_file_add or zip_file_replace call.
            if (zip_set_file_compression(zip_archive, index, METHOD, COMPRESSION) < 0) {
                printf("error setting compression settings for static archive (name + data) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }
            if (zip_file_set_comment(zip_archive, index, "blake3", strlen("blake3"), ZIP_FL_ENC_UTF_8) < 0) {
                printf("error setting comment for static archive (name + data) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }
        }
    }
    if (sys_libs.size() != 0) {
        buffers.emplace_back("");
        std::string & libs = buffers.back();

        for (int i = 0; i < sys_libs.size(); i++) {
            libs += "-l" + sys_libs[i];
            if (i+1 < sys_libs.size()) libs += " ";
        }

        printf("adding system libraries (name only) : '%s'\n", libs.c_str());

        zip_source_t * zip_src = zip_source_buffer(zip_archive, libs.c_str(), libs.size(), 0);
        if (zip_src == nullptr) {
            printf("error creaing buffer for system library (name only) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            goto ZIP_ARCHIVE_EXIT;
        }
        zip_int64_t index = zip_file_add(zip_archive, "SYSTEM_LIBRARIES", zip_src, ZIP_FL_ENC_UTF_8);
        if (index < 0) {
            printf("error adding system library (name only) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            zip_source_free(zip_src);
            goto ZIP_ARCHIVE_EXIT;
        }
        // zip_source_free(3) should not be called on a source after it was used
        //  successfully in a zip_file_add or zip_file_replace call.
        if (zip_set_file_compression(zip_archive, index, METHOD, COMPRESSION) < 0) {
            printf("error setting compression settings for system library (name only) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            goto ZIP_ARCHIVE_EXIT;
        }
        if (zip_file_set_comment(zip_archive, index, "text", strlen("text"), ZIP_FL_ENC_UTF_8) < 0) {
            printf("error setting comment for system library (name only) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            goto ZIP_ARCHIVE_EXIT;
        }

        // BLAKE3
        {
            printf("writing hash...\n");

            // Initialize the hasher.
            llvm::BLAKE3 hasher;

            // Read input bytes from stdin.
            hasher.update(llvm::StringRef(libs.c_str(), libs.size()));

            // Finalize the hash. Default output length is 32 bytes.
            buffer_hashes.emplace_back(hasher.final());
            llvm::BLAKE3Result<LLVM_BLAKE3_OUT_LEN> & output = buffer_hashes.back();

            printf("wrote hash\n");

            // Print the hash as hexadecimal.
            printf("hash: ");
            for (uint8_t byte : output) {
                printf("%02x", byte);
            }
            printf("\n");

            File file("/tmp/SYSTEM_LIBRARIES.blake3", "w+");
            if (file.valid()) {
                file.write(output.data(), output.size());
                file.close();
            }

            buffers.emplace_back("SYSTEM_LIBRARIES.blake3");
            std::string & HASH = buffers.back();

            zip_source_t * zip_src = zip_source_buffer(zip_archive, output.data(), output.size(), 0);
            if (zip_src == nullptr) {
                printf("error reading system library (name only) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }
            zip_int64_t index = zip_file_add(zip_archive, HASH.c_str(), zip_src, ZIP_FL_ENC_UTF_8);
            if (index < 0) {
                printf("error adding system library (name only) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                zip_source_free(zip_src);
                goto ZIP_ARCHIVE_EXIT;
            }
            // zip_source_free(3) should not be called on a source after it was used
            //  successfully in a zip_file_add or zip_file_replace call.
            if (zip_set_file_compression(zip_archive, index, METHOD, COMPRESSION) < 0) {
                printf("error setting compression settings for system library (name only) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }
            if (zip_file_set_comment(zip_archive, index, "blake3", strlen("blake3"), ZIP_FL_ENC_UTF_8) < 0) {
                printf("error setting comment for system library (name only) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }
        }
    }
    for (int i = 0; i < executables.size(); i++) {
        printf("adding executable (name + data) : name = '%s', path = '%s'\n", executables[i].first.c_str(), executables[i].second.c_str());
        zip_source_t * zip_src = zip_source_file(zip_archive, executables[i].second.c_str(), 0, ZIP_LENGTH_TO_END);
        if (zip_src == nullptr) {
            printf("error reading executable (name + data) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            goto ZIP_ARCHIVE_EXIT;
        }
        zip_int64_t index = zip_file_add(zip_archive, executables[i].first.c_str(), zip_src, ZIP_FL_ENC_UTF_8);
        if (index < 0) {
            printf("error adding executable (name + data) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            zip_source_free(zip_src);
            goto ZIP_ARCHIVE_EXIT;
        }
        // zip_source_free(3) should not be called on a source after it was used
        //  successfully in a zip_file_add or zip_file_replace call.
        if (zip_set_file_compression(zip_archive, index, METHOD, COMPRESSION) < 0) {
            printf("error setting compression settings for executable (name + data) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            goto ZIP_ARCHIVE_EXIT;
        }
        if (zip_file_set_comment(zip_archive, index, "exe", strlen("exe"), ZIP_FL_ENC_UTF_8) < 0) {
            printf("error setting comment for executable (name + data) : %s\n", zip_strerror(zip_archive));
            zip_error_code = 1;
            goto ZIP_ARCHIVE_EXIT;
        }

        // BLAKE3
        {
            printf("writing hash...\n");
            FILE* f = fopen(executables[i].second.c_str(), "r");
            if (f == nullptr) {
                printf("error opening disk zip file: %s\n", strerror(errno));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }

            // Initialize the hasher.
            llvm::BLAKE3 hasher;

            // Read input bytes from stdin.
            char buf[65536];
            while (1) {
                ssize_t n = fread(buf, 1, sizeof(buf), f);
                if (n > 0) {
                    hasher.update(llvm::StringRef(buf, n));
                } else if (n == 0) {
                    break; // end of file
                } else {
                    printf("read failed: %s\n", strerror(errno));
                    fclose(f);
                    goto ZIP_ARCHIVE_EXIT;
                }
            }

            fclose(f);

            // Finalize the hash. Default output length is 32 bytes.
            buffer_hashes.emplace_back(hasher.final());
            llvm::BLAKE3Result<LLVM_BLAKE3_OUT_LEN> & output = buffer_hashes.back();

            printf("wrote hash\n");

            // Print the hash as hexadecimal.
            printf("hash: ");
            for (uint8_t byte : output) {
                printf("%02x", byte);
            }
            printf("\n");

            buffers.emplace_back(executables[i].first + ".blake3");
            std::string & HASH = buffers.back();

            zip_source_t * zip_src = zip_source_buffer(zip_archive, output.data(), output.size(), 0);
            if (zip_src == nullptr) {
                printf("error reading executable (name + data) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }
            zip_int64_t index = zip_file_add(zip_archive, HASH.c_str(), zip_src, ZIP_FL_ENC_UTF_8);
            if (index < 0) {
                printf("error adding executable (name + data) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                zip_source_free(zip_src);
                goto ZIP_ARCHIVE_EXIT;
            }
            // zip_source_free(3) should not be called on a source after it was used
            //  successfully in a zip_file_add or zip_file_replace call.
            if (zip_set_file_compression(zip_archive, index, METHOD, COMPRESSION) < 0) {
                printf("error setting compression settings for executable (name + data) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }
            if (zip_file_set_comment(zip_archive, index, "blake3", strlen("blake3"), ZIP_FL_ENC_UTF_8) < 0) {
                printf("error setting comment for executable (name + data) hash : %s\n", zip_strerror(zip_archive));
                zip_error_code = 1;
                goto ZIP_ARCHIVE_EXIT;
            }
        }
    }

    // close zip archive
    printf("writing compressed zip archive...\n");
    if (zip_close(zip_archive) < 0) {
        printf("error writing in-memory zip archive: %s\n", zip_strerror(zip_archive));
        zip_error_code = 1;
        goto ZIP_MEM_EXIT;
    }
    printf("\nwrote compressed zip archive\n");

    if (zip_source_is_deleted(zip_memory)) {
        printf("in-memory zip buffer is empty\n");
        zip_error_code = 1;
        goto ZIP_MEM_EXIT;
    }

    // write zip to memory
    if (zip_source_open(zip_memory) < 0) {
        printf("error opening in-memory zip buffer: %s\n", zip_error_strerror(zip_source_error(zip_memory)));
        zip_error_code = 1;
        goto ZIP_MEM_EXIT;
    }
    {
        if (zip_source_seek(zip_memory, 0, SEEK_END) < 0) {
            printf("error seeking in-memory zip buffer: %s\n", zip_error_strerror(zip_source_error(zip_memory)));
            zip_error_code = 1;
            goto ZIP_MEM_OPEN_EXIT;
        }
        {
            zip_int64_t sz = zip_source_tell(zip_memory);
            if (sz < 0) {
                printf("error obtaining size of in-memory zip buffer: %s\n", zip_error_strerror(zip_source_error(zip_memory)));
                zip_error_code = 1;
                goto ZIP_MEM_OPEN_EXIT;
            }
            if (zip_source_seek(zip_memory, 0, SEEK_SET) < 0) {
                printf("error seeking in-memory zip buffer: %s\n", zip_error_strerror(zip_source_error(zip_memory)));
                zip_error_code = 1;
                goto ZIP_MEM_OPEN_EXIT;
            }
            {
                std::string split_c_obtain_total_size;
                std::string split_c_obtain_complete_archive_h;
                std::string split_c_obtain_complete_archive;
                std::string split_c_obtain_complete_archive_print_contents_exe;
                FILE * zip_file = nullptr;
                FILE * split_zip_file_obtain_total_size = nullptr;
                FILE * split_zip_file_obtain_complete_archive_h = nullptr;
                FILE * split_zip_file_obtain_complete_archive = nullptr;
                FILE * split_zip_file_obtain_complete_archive_print_contents_exe = nullptr;

                // old behaviour:
                //   write to c - OUTPUT_FILE
                //   compile c
                //
                // new behaviour:
                //   write to cmake file - OUTPUT_CMAKE
                //   cmake produces OUTPUT_FILE
                //
                zip_file = fopen(cmake_file.c_str(), "w+");
                if (zip_file == nullptr) {
                    printf("error opening disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_MEM_OPEN_EXIT;
                }

                if (fprintf(zip_file, "cmake_minimum_required(VERSION 3.5)\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                if (fprintf(zip_file, "project(%s C CXX)\n", data_label.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                if (fprintf(zip_file, "set(CMAKE_C_STANDARD 11)\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                if (fprintf(zip_file, "set(CMAKE_C_STANDARD_REQUIRED ON)\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                // we have our cmake header

                // now, generate our compilation files...

                if (fprintf(zip_file, "add_library(out STATIC \n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                split_c_obtain_total_size =
                    output_file_dir
                    + "/"
                    + "split_c_total_size.cpp";
                if (fprintf(zip_file, "    %s\n", split_c_obtain_total_size.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                split_zip_file_obtain_total_size = fopen(split_c_obtain_total_size.c_str(), "w+");
                if (split_zip_file_obtain_total_size == nullptr) {
                    printf("error opening disk zip file '%s' : %s\n", split_c_obtain_total_size.c_str(), strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_total_size, "#include <zip.h>\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                if (fprintf(split_zip_file_obtain_total_size, "zip_int64_t %s_total_file_size = %" PRId64 ";\n", data_label.c_str(), sz) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                split_c_obtain_complete_archive_h =
                    output_file_dir
                    + "/"
                    + "split_c_archive.h";
                split_zip_file_obtain_complete_archive_h = fopen(split_c_obtain_complete_archive_h.c_str(), "w+");
                if (split_zip_file_obtain_complete_archive_h == nullptr) {
                    printf("error opening disk zip file '%s' : %s\n", split_c_obtain_complete_archive_h.c_str(), strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_complete_archive_h, "#include <zip.h>\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_complete_archive_h, "#include <stdlib.h> // malloc,free\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_complete_archive_h, "#include <string.h> // memcpy\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_complete_archive_h, "extern int %s_zip_sys_libs_size;\n", data_label.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_complete_archive_h, "extern const char * %s_zip_sys_libs[%d];", data_label.c_str(), (int)sys_libs.size()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_complete_archive_h, "extern zip_int64_t %s_total_file_size;\n", data_label.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_complete_archive_h, "extern void * %s_total_file_obtain();\n", data_label.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_complete_archive_h, "extern void %s_total_file_release(void * zip_data);\n", data_label.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                split_c_obtain_complete_archive =
                    output_file_dir
                    + "/"
                    + "split_c_archive.cpp";
                if (fprintf(zip_file, "    %s\n", split_c_obtain_complete_archive.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                split_zip_file_obtain_complete_archive = fopen(split_c_obtain_complete_archive.c_str(), "w+");
                if (split_zip_file_obtain_complete_archive == nullptr) {
                    printf("error opening disk zip file '%s' : %s\n", split_c_obtain_complete_archive.c_str(), strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                {
                    std::string header = "split_c_archive.h";
                    if (fprintf(split_zip_file_obtain_complete_archive, "#include \"%s\"\n", header.c_str()) < 0) {
                        printf("error writing to disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                        goto ZIP_EXIT;
                    }
                }
                if (fprintf(split_zip_file_obtain_complete_archive, "void * %s_total_file_obtain() {\n", data_label.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_complete_archive, "    void * zip_data = malloc(%s_total_file_size);\n", data_label.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(split_zip_file_obtain_complete_archive, "    if (zip_data == nullptr) return nullptr;\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                {
                    int split_count = 1;
                    zip_int64_t sz_remaining = sz;
                    FILE * split_zip_file = nullptr;

                    while (true) {
                        std::string split_c_file = "split_" + std::to_string(split_count) + ".cpp";
                        std::string split_c = output_file_dir + "/" + split_c_file;
                        split_zip_file = fopen(split_c.c_str(), "w+");
                        if (split_zip_file == nullptr) {
                            printf("error opening disk split zip file '%s' : %s\n", split_c.c_str(), strerror(errno));
                            zip_error_code = 1;
                            sz_remaining = 0;
                            goto ZIP_EXIT;
                        }

                        if (fprintf(zip_file, "    %s\n", split_c.c_str()) < 0) {
                            printf("error writing to disk zip file: %s\n", strerror(errno));
                            zip_error_code = 1;
                            sz_remaining = 0;
                            goto SPLIT_ZIP_EXIT;
                        }

                        // split 1 is always the system libs

                        if (fprintf(split_zip_file, "#include <stddef.h>\n") < 0) {
                            printf("error writing to disk split zip file: %s\n", strerror(errno));
                            zip_error_code = 1;
                            sz_remaining = 0;
                            goto SPLIT_ZIP_EXIT;
                        }

                        if (split_count == 1) {
                            if (fprintf(split_zip_file, "int %s_zip_sys_libs_size = %d;\n", data_label.c_str(), (int)sys_libs.size()) < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                            if (fprintf(split_zip_file, "const char * %s_zip_sys_libs[%d] = {", data_label.c_str(), (int)sys_libs.size()) < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                            for (int i = 0; i < sys_libs.size(); i++) {
                                if (fprintf(split_zip_file, "\"%s\"", sys_libs[i].c_str()) < 0) {
                                    printf("error writing to disk split zip file: %s\n", strerror(errno));
                                    zip_error_code = 1;
                                    sz_remaining = 0;
                                    goto SPLIT_ZIP_EXIT;
                                }
                                if (i+1 != sys_libs.size()) {
                                    if (fprintf(split_zip_file, ", ") < 0) {
                                        printf("error writing to disk split zip file: %s\n", strerror(errno));
                                        zip_error_code = 1;
                                        sz_remaining = 0;
                                        goto SPLIT_ZIP_EXIT;
                                    }
                                }
                            }
                            if (fprintf(split_zip_file, "};\n") < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                        } else {
                            if (fprintf(split_zip_file, "#include <inttypes.h>\n") < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                            // split 2 and higher are static archives
                            int buffer_size = sz_remaining < (1024*2048) ? (int) sz_remaining : (1024*2048);
                            if (fprintf(split_zip_file, "extern const int %s_zip_size_split_%d = %d;\n", data_label.c_str(), split_count-1, buffer_size) < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                            if (fprintf(split_zip_file_obtain_complete_archive_h, "extern const int %s_zip_size_split_%d;\n", data_label.c_str(), split_count-1) < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                            if (fprintf(split_zip_file_obtain_complete_archive_h, "extern const uint8_t %s_zip_split_%d[%d];\n", data_label.c_str(), split_count-1, buffer_size+1) < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                            if (split_count == 2) {
                                if (fprintf(split_zip_file_obtain_complete_archive, "    uint8_t* zip_data_head = (uint8_t*)zip_data;\n") < 0) {
                                    printf("error writing to disk split zip file: %s\n", strerror(errno));
                                    zip_error_code = 1;
                                    sz_remaining = 0;
                                    goto SPLIT_ZIP_EXIT;
                                }
                            }
                            if (fprintf(split_zip_file_obtain_complete_archive, "    memcpy(zip_data_head, %s_zip_split_%d, %s_zip_size_split_%d);\n", data_label.c_str(), split_count-1, data_label.c_str(), split_count-1) < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                            if (fprintf(split_zip_file_obtain_complete_archive, "    zip_data_head += %s_zip_size_split_%d;\n", data_label.c_str(), split_count-1) < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                            if (fprintf(split_zip_file, "extern const uint8_t %s_zip_split_%d[%d] = \"", data_label.c_str(), split_count-1, buffer_size+1) < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                            {
                                int written = 0;
                                while(written != buffer_size) {
                                    uint8_t buf[buffer_size];
                                    int r = (int) zip_source_read(zip_memory, buf, buffer_size);
                                    if (r < 0) {
                                        printf("error reading in-memory zip buffer: %s\n", zip_error_strerror(zip_source_error(zip_memory)));
                                        zip_error_code = 1;
                                        sz_remaining = 0;
                                        goto SPLIT_ZIP_EXIT;
                                    }
                                    for (int i = 0; i < r; i++) {
                                        if (fprintf(split_zip_file, "\\x%02X", buf[i]) < 0) {
                                            printf("error writing to disk split zip file: %s\n", strerror(errno));
                                            zip_error_code = 1;
                                            sz_remaining = 0;
                                            goto SPLIT_ZIP_EXIT;
                                        }
                                    }
                                    written += r;
                                }
                            }
                            if (fprintf(split_zip_file, "\";\n") < 0) {
                                printf("error writing to disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                sz_remaining = 0;
                                goto SPLIT_ZIP_EXIT;
                            }
                            sz_remaining -= buffer_size;
                            printf("written %d bytes (%" PRId64 " bytes of %" PRId64 " total bytes) to '%s'\n", buffer_size, (sz-sz_remaining), sz, split_c_file.c_str());
                        }

                        SPLIT_ZIP_EXIT:
                        if (split_zip_file != nullptr) {
                            if (fflush(split_zip_file) < 0) {
                                printf("error flushing disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                goto ZIP_EXIT;
                            }
#ifdef _WIN32
                            HANDLE f = _get_osfhandle(_fileno(split_zip_file));
                            FlushFileBuffers(f);
                            split_zip_file = _fdopen(_open_osfhandle(f, _O_APPEND), "r+");
#else
                            if (fsync(fileno(split_zip_file)) < 0) {
                                printf("error syncing disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                goto ZIP_EXIT;
                            }
#endif
                            if (fclose(split_zip_file) < 0) {
                                printf("error closing disk split zip file: %s\n", strerror(errno));
                                zip_error_code = 1;
                                goto ZIP_EXIT;
                            }
                        }
                        if (sz_remaining != 0) {
                            split_count++;
                            continue;
                        } else {
                            if (zip_error_code == 1) {
                                goto ZIP_EXIT;
                            } else {
                                break;
                            }
                        }
                    }
                }

                if (fprintf(split_zip_file_obtain_complete_archive, "    return zip_data;\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                if (fprintf(split_zip_file_obtain_complete_archive, "}\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                if (fprintf(split_zip_file_obtain_complete_archive, "void %s_total_file_release(void * zip_data) {\n", data_label.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                if (fprintf(split_zip_file_obtain_complete_archive, "    if (zip_data != nullptr) free(zip_data);\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                if (fprintf(split_zip_file_obtain_complete_archive, "}\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                if (fprintf(zip_file, ")\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                for (size_t i = 0; i < cmake_link_libs.size(); i++) {
                    if (fprintf(zip_file, "add_library(zip_%zu STATIC IMPORTED)\n", i) < 0) {
                        printf("error writing to disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                        goto ZIP_EXIT;
                    }
                    if (fprintf(zip_file, "set_target_properties(zip_%zu PROPERTIES IMPORTED_LOCATION %s)\n", i, cmake_link_libs[i].c_str()) < 0) {
                        printf("error writing to disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                        goto ZIP_EXIT;
                    }
                    if (fprintf(zip_file, "target_link_libraries(out PUBLIC zip_%zu)\n", i) < 0) {
                        printf("error writing to disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                        goto ZIP_EXIT;
                    }
                }

                for (size_t i = 0; i < cmake_link_includes.size(); i++) {
                    if (fprintf(zip_file, "target_include_directories(out PUBLIC %s)\n", cmake_link_includes[i].c_str()) < 0) {
                        printf("error writing to disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                        goto ZIP_EXIT;
                    }
                }

                if (fprintf(zip_file, "set_target_properties(out PROPERTIES PREFIX \"\")\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(zip_file, "set_target_properties(out PROPERTIES OUTPUT_NAME %s)\n", output_file_name.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(zip_file, "set_target_properties(out PROPERTIES SUFFIX .%s)\n", output_file_suffix.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }

                split_c_obtain_complete_archive_print_contents_exe =
                    output_file_dir
                    + "/"
                    + "split_c_print_contents_exe.cpp";
                if (fprintf(zip_file, "add_executable(out_exe %s)\n", split_c_obtain_complete_archive_print_contents_exe.c_str()) < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (fprintf(zip_file, "target_link_libraries(out_exe out)\n") < 0) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                split_zip_file_obtain_complete_archive_print_contents_exe = fopen(split_c_obtain_complete_archive_print_contents_exe.c_str(), "w+");
                if (split_zip_file_obtain_complete_archive_print_contents_exe == nullptr) {
                    printf("error opening disk zip file '%s' : %s\n", split_c_obtain_complete_archive_print_contents_exe.c_str(), strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                if (
                    fprintf(split_zip_file_obtain_complete_archive_print_contents_exe,
                        "#include <config.h>\n"
                        "#include <compat.h>\n"
                        "#include \"split_c_archive.h\"\n"
                        "int main() {\n"
                        "    printf(\"archive size: %%\" PRId64 \" bytes\\n\", %s_total_file_size);\n"
                        "    printf(\"obtaining archive data...\\n\");\n"
                        "    void * zip_data = %s_total_file_obtain();\n"
                        "    printf(\"obtained archive data\\n\");\n"
                        "    printf(\"reading archive data...\\n\");\n"
                        "    zip_int64_t zip_data_size = %s_total_file_size;\n"
                        "    printf(\"obtained archive data\\n\");\n"
                        "    int zip_error_code = 0;\n"
                        "    zip_error_t error;\n"
                        "    zip * zip_archive;\n"
                        "    zip_int64_t count = 0;\n"
                        "    zip_error_init(&error);\n"
                        "    zip_source_t * zip_memory = zip_source_buffer_create(zip_data, zip_data_size, 0, &error);\n"
                        "    if (zip_memory == nullptr) {\n"
                        "        printf(\"error creating in-memory zip buffer: %%s\\n\", zip_error_strerror(&error));\n"
                        "        zip_error_code = 1;\n"
                        "        zip_error_fini(&error);\n"
                        "        goto ZIP_DATA_EXIT;\n"
                        "    }\n"
                        "    zip_error_fini(&error);\n"
                        "    zip_error_init(&error);\n"
                        "    zip_archive = zip_open_from_source(zip_memory, ZIP_CHECKCONS, &error);\n"
                        "    if (zip_archive == nullptr) {\n"
                        "        printf(\"error opening in-memory zip archive: %%s\\n\", zip_error_strerror(&error));\n"
                        "        zip_error_code = 1;\n"
                        "        zip_error_fini(&error);\n"
                        "        goto ZIP_MEM_EXIT;\n"
                        "    }\n"
                        "    zip_error_fini(&error);\n"
                        "    zip_source_keep(zip_memory);\n"
                        "    printf(\"archive contents:\\n\");\n"
                        "    count = zip_get_num_entries(zip_archive, 0);\n"
                        "    printf(\"%%\" PRId64 \" entr%%s in archive\\n\", count, count == 1 ? \"y\" : \"ies\");\n"
                        "    for (zip_int64_t idx = 0; idx < count; idx++) {\n"
                        "        struct zip_stat sb;\n"
                        "        char buf[100];\n"
                        "        if (zip_stat_index(zip_archive, idx, ZIP_FL_ENC_UTF_8, &sb) < 0) {\n"
                        "            printf(\"error stat'ing entry of in-memory zip archive: %%s\\n\", zip_strerror(zip_archive));\n"
                        "            continue;\n"
                        "        }\n"
                        "        if (sb.valid & ZIP_STAT_NAME)\n"
                        "            printf(\"name:               '%%s'\\n\", sb.name);\n"
                        "        if (sb.valid & ZIP_STAT_INDEX)\n"
                        "            printf(\"index:              '%%\" PRIu64 \"'\\n\", sb.index);\n"
                        "        if (sb.valid & ZIP_STAT_SIZE)\n"
                        "            printf(\"size:               '%%\" PRIu64 \"'\\n\", sb.size);\n"
                        "        if (sb.valid & ZIP_STAT_COMP_SIZE)\n"
                        "            printf(\"compressed size:    '%%\" PRIu64 \"'\\n\", sb.comp_size);\n"
                        "        if (sb.valid & ZIP_STAT_SIZE && sb.valid & ZIP_STAT_COMP_SIZE)\n"
                        "            printf(\"compression ratio:  '%%d%%%%'\\n\", 100-(int)(((double)sb.comp_size / (double)sb.size)*100));\n"
                        "        if (sb.valid & ZIP_STAT_MTIME) {\n"
                        "            struct tm *tpm;\n"
                        "            struct tm tm;\n"
                        "            tpm = zip_localtime(&sb.mtime, &tm);\n"
                        "            if (tpm == NULL) {\n"
                        "                printf(\"mtime:              <not valid>\\n\");\n"
                        "            } else {\n"
                        "                strftime(buf, sizeof(buf), \"%%a %%b %%d %%Y %%H:%%M:%%S\", tpm);\n"
                        "                printf(\"mtime:              '%%s'\\n\", buf);\n"
                        "            }\n"
                        "        }\n"
                        "        if (sb.valid & ZIP_STAT_CRC)\n"
                        "            printf(\"crc:                '%%0x'\\n\", sb.crc);\n"
                        "        if (sb.valid & ZIP_STAT_COMP_METHOD)\n"
                        "            printf(\"compression method: '%%d'\\n\", sb.comp_method);\n"
                        "        if (sb.valid & ZIP_STAT_ENCRYPTION_METHOD)\n"
                        "            printf(\"encryption method:  '%%d'\\n\", sb.encryption_method);\n"
                        "        if (sb.valid & ZIP_STAT_FLAGS)\n"
                        "            printf(\"flags:              '%%ld'\\n\", (long)sb.flags);\n"
                        "        const char * comment = zip_file_get_comment(zip_archive, idx, nullptr, 0);\n"
                        "        printf(\"comment:            '%%s'\\n\", comment == nullptr ? \"<no comment was specified>\" : comment);\n"
                        "        printf(\"\\n\");\n"
                        "    }\n"
                        "    ZIP_ARCHIVE_EXIT:\n"
                        "    zip_discard(zip_archive);\n"
                        "    ZIP_MEM_EXIT:\n"
                        "    zip_source_free(zip_memory);\n"
                        "    ZIP_DATA_EXIT:\n"
                        "    %s_total_file_release(zip_data);\n"
                        "    return zip_error_code;\n"
                        "}\n",
                        data_label.c_str(),
                        data_label.c_str(),
                        data_label.c_str(),
                        data_label.c_str()
                    ) < 0
                ) {
                    printf("error writing to disk zip file: %s\n", strerror(errno));
                    zip_error_code = 1;
                    goto ZIP_EXIT;
                }
                
                ZIP_EXIT:
                if (split_zip_file_obtain_complete_archive_print_contents_exe != nullptr) {
                    if (fflush(split_zip_file_obtain_complete_archive_print_contents_exe) < 0) {
                        printf("error flushing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
#ifdef _WIN32
                    HANDLE f = _get_osfhandle(_fileno(split_zip_file_obtain_complete_archive_print_contents_exe));
                    FlushFileBuffers(f);
                    split_zip_file_obtain_complete_archive_print_contents_exe = _fdopen(_open_osfhandle(f, _O_APPEND), "r+");
#else
                    if (fsync(fileno(split_zip_file_obtain_complete_archive_print_contents_exe)) < 0) {
                        printf("error syncing disk file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
#endif
                    if (fclose(split_zip_file_obtain_complete_archive_print_contents_exe) < 0) {
                        printf("error closing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
                }
                if (split_zip_file_obtain_total_size != nullptr) {
                    if (fflush(split_zip_file_obtain_total_size) < 0) {
                        printf("error flushing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
#ifdef _WIN32
                    HANDLE f = _get_osfhandle(_fileno(split_zip_file_obtain_total_size));
                    FlushFileBuffers(f);
                    split_zip_file_obtain_total_size = _fdopen(_open_osfhandle(f, _O_APPEND), "r+");
#else
                    if (fsync(fileno(split_zip_file_obtain_total_size)) < 0) {
                        printf("error syncing disk file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
#endif
                    if (fclose(split_zip_file_obtain_total_size) < 0) {
                        printf("error closing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
                }
                if (split_zip_file_obtain_complete_archive_h != nullptr) {
                    if (fflush(split_zip_file_obtain_complete_archive_h) < 0) {
                        printf("error flushing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
#ifdef _WIN32
                    HANDLE f = _get_osfhandle(_fileno(split_zip_file_obtain_complete_archive_h));
                    FlushFileBuffers(f);
                    split_zip_file_obtain_complete_archive_h = _fdopen(_open_osfhandle(f, _O_APPEND), "r+");
#else
                    if (fsync(fileno(split_zip_file_obtain_complete_archive_h)) < 0) {
                        printf("error syncing disk file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
#endif
                    if (fclose(split_zip_file_obtain_complete_archive_h) < 0) {
                        printf("error closing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
                }
                if (split_zip_file_obtain_complete_archive != nullptr) {
                    if (fflush(split_zip_file_obtain_complete_archive) < 0) {
                        printf("error flushing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
#ifdef _WIN32
                    HANDLE f = _get_osfhandle(_fileno(split_zip_file_obtain_complete_archive));
                    FlushFileBuffers(f);
                    split_zip_file_obtain_complete_archive = _fdopen(_open_osfhandle(f, _O_APPEND), "r+");
#else
                    if (fsync(fileno(split_zip_file_obtain_complete_archive)) < 0) {
                        printf("error syncing disk file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
#endif
                    if (fclose(split_zip_file_obtain_complete_archive) < 0) {
                        printf("error closing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
                }
                if (zip_file != nullptr) {
                    if (fflush(zip_file) < 0) {
                        printf("error flushing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
#ifdef _WIN32
                    HANDLE f = _get_osfhandle(_fileno(zip_file));
                    FlushFileBuffers(f);
                    zip_file = _fdopen(_open_osfhandle(f, _O_APPEND), "r+");
#else
                    if (fsync(fileno(zip_file)) < 0) {
                        printf("error syncing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
#endif
                    if (fclose(zip_file) < 0) {
                        printf("error closing disk zip file: %s\n", strerror(errno));
                        zip_error_code = 1;
                    }
                }
            }
        }

        ZIP_MEM_OPEN_EXIT:
        if (zip_source_close(zip_memory) < 0) {
            printf("error closing in-memory zip buffer: %s\n", zip_error_strerror(zip_source_error(zip_memory)));
            zip_error_code = 1;
        }
        goto ZIP_MEM_EXIT;
    }

    ZIP_ARCHIVE_EXIT:
    zip_discard(zip_archive);

    ZIP_MEM_EXIT:
    zip_source_free(zip_memory);

    return zip_error_code;
}
