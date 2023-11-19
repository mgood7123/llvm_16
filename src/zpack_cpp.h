#include <zpack_common.h>
#include <zpack.h>

namespace ZPack {

    struct ErrorMessage {
        const char * error_msg;
        std::string error_msg_string;
        inline const char * zpack_error_to_string(int ret) {
            static const char * zpack_error_strings[] //! Return codes
            = {
                "ZPACK_OK",                         //!< No errors

                "ZPACK_ERROR_ARCHIVE_NOT_LOADED",   //!< Archive has not been loaded
                "ZPACK_ERROR_WRITER_NOT_OPENED",    //!< Writer has not been opened
                "ZPACK_ERROR_OPEN_FAILED",          //!< Failed to open file
                "ZPACK_ERROR_SEEK_FAILED",          //!< Failed to seek file
                "ZPACK_ERROR_FILE_TOO_SMALL",       //!< File size too small to be an archive
                "ZPACK_ERROR_SIGNATURE_INVALID",    //!< Invalid archive signature
                "ZPACK_ERROR_READ_FAILED",          //!< Failed to read data from the archive
                "ZPACK_ERROR_BLOCK_SIZE_INVALID",   //!< Invalid block size
                "ZPACK_ERROR_VERSION_INCOMPATIBLE", //!< Archive version is not supported
                "ZPACK_ERROR_MALLOC_FAILED",        //!< Failed to allocate memory
                "ZPACK_ERROR_FILE_NOT_FOUND",       //!< Could not find file in archive
                "ZPACK_ERROR_BUFFER_TOO_SMALL",     //!< Buffer size is too small
                "ZPACK_ERROR_DECOMPRESS_FAILED",    //!< Decompression error (check last_return for the compression library's error code)
                "ZPACK_ERROR_COMPRESS_FAILED",      //!< Compression error (check last_return for the compression library's error code)
                "ZPACK_ERROR_FILE_HASH_MISMATCH",   //!< The decompressed file's hash does not match the original file's hash
                "ZPACK_ERROR_FILE_OFFSET_INVALID",  //!< Invalid file offset
                "ZPACK_ERROR_FILE_INCOMPLETE",      //!< The file's data is incomplete
                "ZPACK_ERROR_FILE_SIZE_INVALID",    //!< Invalid file size
                "ZPACK_ERROR_COMP_METHOD_INVALID",  //!< Invalid compression method
                "ZPACK_ERROR_WRITE_FAILED",         //!< Failed to write data to file
                "ZPACK_ERROR_STREAM_INVALID",       //!< Invalid stream
                "ZPACK_ERROR_HASH_FAILED",          //!< Failed to generate hash for the data provided
                "ZPACK_ERROR_FILENAME_TOO_LONG",    //!< Filename length exceeds limit (65535 characters)
                "ZPACK_ERROR_NOT_AVAILABLE"         //!< Feature not available in this build of ZPack (compression method disabled, etc.)
            };
            return zpack_error_strings[ret];
        }
        virtual inline ~ErrorMessage() {};
    };

    struct Reader : public ErrorMessage {
        //virtual void read(zpack_stream & stream, zpack_file & file) = 0;
        virtual inline ~Reader() {};
    };

    class HeapReader : public Reader {

        zpack_reader reader;

        public:
        inline HeapReader() {
            error_msg = nullptr;
            memset(&reader, 0, sizeof(zpack_reader));
        }

        inline void open(zpack_u8 * buffer, size_t size) {
            int ret;
            if ((ret = zpack_init_reader_memory_shared(&reader, buffer, size))) {
                error_msg_string = "ZPack: Failed to open memory for reading";
                error_msg_string += ", Error: ";
                error_msg_string += zpack_error_to_string(ret);
                error_msg = error_msg_string.c_str();
                return;
            }
        }

        inline void print() {
            zpack_file_entry* entries = reader.file_entries;
            printf("ZPack: File count: %" PRIu64 "\n\n", reader.file_count);

            for (zpack_u64 i = 0; i < reader.file_count; ++i)
            {
                printf("ZPack: File #%" PRIu64 "\n"
                    "ZPack: Filename: %s\n"
                    "ZPack: Compressed size: %" PRIu64 "\n"
                    "ZPack: Uncompressed size: %" PRIu64 "\n"
                    "ZPack: File hash: %" PRIx64 "\n"
                    "ZPack: Compression method: %u\n",
                i + 1, entries[i].filename, entries[i].comp_size, entries[i].uncomp_size,
                entries[i].hash, entries[i].comp_method);
            }
        }

        inline ~HeapReader() {
            zpack_close_reader(&reader);
        }
    };

    struct Writer : public ErrorMessage {
        virtual void write(zpack_stream & stream, zpack_file & file) = 0;
        virtual inline ~Writer() {};
    };
    
    class HeapWriter : public Writer {

        zpack_writer writer;

        public:
        inline HeapWriter() {
            error_msg = nullptr;
            memset(&writer, 0, sizeof(zpack_writer));
            int ret;
            if ((ret = zpack_init_writer_heap(&writer, 0))) {
                error_msg_string = "ZPack: Failed to create in-memory pack for writing";
                error_msg_string += ", Error: ";
                error_msg_string += zpack_error_to_string(ret);
                error_msg = error_msg_string.c_str();
                return;
            }

            if ((ret = zpack_write_header(&writer))) {
                error_msg_string = "ZPack: Failed to write archive header";
                error_msg_string += ", Error: ";
                error_msg_string += zpack_error_to_string(ret);
                error_msg = error_msg_string.c_str();
                return;
            }

            if ((ret = zpack_write_data_header(&writer))) {
                error_msg_string = "ZPack: Failed to write archive data header";
                error_msg_string += ", Error: ";
                error_msg_string += zpack_error_to_string(ret);
                error_msg = error_msg_string.c_str();
                return;
            }
        }

        inline void write(zpack_stream & stream, zpack_file & file) override {
            int ret;
            while (stream.total_in < file.size) {
                stream.avail_in = ZPACK_MIN(16, file.size - stream.total_in);
                if ((ret = zpack_write_file_stream(&writer, file.options, &stream, nullptr))) {
                    error_msg_string = "ZPack: Failed to write to stream";
                    error_msg_string += ", Error: ";
                    error_msg_string += zpack_error_to_string(ret);
                    error_msg = error_msg_string.c_str();
                    return;
                }
            }

            if ((ret = zpack_write_file_stream_end(&writer, file.filename, file.options, &stream, nullptr)))
            {
                error_msg_string = "ZPack: Failed to write to stream";
                error_msg_string += ", Error: ";
                error_msg_string += zpack_error_to_string(ret);
                error_msg = error_msg_string.c_str();
                return;
            }
        }

        inline void finalize(zpack_u8 ** out_buffer, size_t * out_size) {
            int ret;

            if ((ret = zpack_write_cdr(&writer)))
            {
                error_msg_string = "ZPack: Failed to write archive central directory record (cdr)";
                error_msg_string += ", Error: ";
                error_msg_string += zpack_error_to_string(ret);
                error_msg = error_msg_string.c_str();
                return;
            }

            if ((ret = zpack_write_eocdr(&writer)))
            {
                error_msg_string = "ZPack: Failed to write archive end of central directory record (eocdr)";
                error_msg_string += ", Error: ";
                error_msg_string += zpack_error_to_string(ret);
                error_msg = error_msg_string.c_str();
                return;
            }

            if (out_buffer != nullptr && out_size != nullptr) {
                *out_size = writer.file_size;
                *out_buffer = (zpack_u8*)malloc(*out_size);
                if (*out_buffer == nullptr) {
                    error_msg_string = "ZPack: Failed to allocate buffer";
                    error_msg = error_msg_string.c_str();
                    return;
                }
                memcpy(*out_buffer, writer.buffer, *out_size);
            }
        }

        inline ~HeapWriter() {
            zpack_close_writer(&writer);
        }

    };

    class Stream : public ErrorMessage {

        zpack_stream stream;

        public:
        inline Stream() {
            error_msg = nullptr;
            int ret;
            memset(&stream, 0, sizeof(zpack_stream));
            if ((ret = zpack_init_stream(&stream))) {
                error_msg_string = "ZPack: Failed to create stream";
                error_msg_string += ", Error: ";
                error_msg_string += zpack_error_to_string(ret);
                error_msg = error_msg_string.c_str();
            }
        }

        inline ~Stream() {
            zpack_close_stream(&stream);
        }

        inline void write(Writer & writer, zpack_file & file) {
            size_t stream_out_size = zpack_get_cstream_out_size(file.options->method);
            
            zpack_u8* out_buf = (zpack_u8*)malloc(sizeof(zpack_u8) * stream_out_size);
            
            if (out_buf == nullptr) {
                error_msg_string = "ZPack: Failed to allocate file buffer";
                error_msg = error_msg_string.c_str();
                return;
            }

            zpack_reset_stream(&stream);
            
            stream.avail_out = sizeof(zpack_u8) * stream_out_size;
            stream.next_out = out_buf;
            stream.next_in = file.buffer;

            writer.write(stream, file);

            free(out_buf);

            if (writer.error_msg != nullptr) {
                error_msg = writer.error_msg;
            }
        }

        class File : public ErrorMessage {

            zpack_file file;
            zpack_compress_options options;
            bool needs_free = false;

            public:

            inline File() {
                error_msg = nullptr;
                int ret;
                memset(&file, 0, sizeof(zpack_file));
            }

            inline ~File() {
                if (needs_free) {
                    if (file.filename != nullptr) {
                        free(file.filename);
                    }
                }
            }

            inline void init(const char * name, void * data, size_t size, zpack_compression_method compression_method, int compression_level) {
                if (name == nullptr) {
                    error_msg_string = "ZPack: filename cannot be null";
                    error_msg = error_msg_string.c_str();
                    return;
                }
                options.method = compression_method;
                options.level = compression_level;

                file.filename = strdup(name);
                if (file.filename == nullptr) {
                    error_msg_string = "ZPack: Failed to allocate memory for duplicate filename";
                    error_msg = error_msg_string.c_str();
                    return;
                }
                needs_free = true;
                file.buffer = (zpack_u8*)data;
                file.size = size;
                file.options = &options;
                file.cctx = nullptr;
            }

            inline void init(char * name, void * data, size_t size, zpack_compression_method compression_method, int compression_level) {
                if (name == nullptr) {
                    error_msg_string = "ZPack: filename cannot be null";
                    error_msg = error_msg_string.c_str();
                    return;
                }
                options.method = compression_method;
                options.level = compression_level;

                file.filename = name;
                file.buffer = (zpack_u8*) data;
                file.size = size;
                file.options = &options;
                file.cctx = nullptr;
            }

            inline void write(Writer & writer, Stream & stream) {
                stream.write(writer, file);
            }
        };

        inline void write(Writer & writer, char * name, void * data, size_t size, zpack_compression_method compression_method, int compression_level) {
            File file;
            file.init(name, data, size, compression_method, compression_level);
            if (file.error_msg != nullptr) {
                error_msg = file.error_msg;
                return;
            }
            file.write(writer, *this);
            if (file.error_msg != nullptr) {
                error_msg = file.error_msg;
            }
        }

        inline void write(Writer & writer, const char * name, void * data, size_t size, zpack_compression_method compression_method, int compression_level) {
            File file;
            file.init(name, data, size, compression_method, compression_level);
            if (file.error_msg != nullptr) {
                error_msg = file.error_msg;
                return;
            }
            file.write(writer, *this);
            if (file.error_msg != nullptr) {
                error_msg = file.error_msg;
            }
        }

        inline void write(Writer & writer, File & file) {
            file.write(writer, *this);
        }
    };
}
