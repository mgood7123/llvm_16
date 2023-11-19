#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct File {
    FILE * file;
    bool written;
    const char * path;
    inline File() {
        file = nullptr;
        written = false;
    }
    inline File(const char * path, const char * mode) {
        file = nullptr;
        written = false;
        open(path, mode);
    }
    inline void open(const char * path, const char * mode) {
        close();
        printf("File::open()\n");
        file = fopen(path, mode);
        this->path = path;
        written = false;
    }
    inline bool valid() { return file != nullptr; }
    inline int read(void * buffer, size_t size) {
        return fread(buffer, 1, size, file);
    }
    inline int write(void * buffer, size_t size) {
        written = true;
        return fwrite(buffer, 1, size, file);
    }
    inline void flush() {
        printf("File::flush()\n");
        if (fflush(file) < 0) {
            printf("error flushing disk file for '%s'\n  error: %s\n", path, strerror(errno));
        }
    #ifdef _WIN32
        HANDLE f = _get_osfhandle(_fileno(file));
        FlushFileBuffers(f);
        file = _fdopen(_open_osfhandle(f, _O_APPEND), "r");
    #else
        if (fsync(fileno(file)) < 0) {
            printf("error syncing disk file for '%s'\n  error: %s\n", path, strerror(errno));
        }
    #endif
    }
    inline void close() {
        if (file != nullptr) {
            if (written) {
                written = false;
            }
            printf("File::close()\n");
            if (fclose(file) < 0) {
                printf("error closing disk file for '%s'\n  error: %s\n", path, strerror(errno));
            }
            file = nullptr;
            path = nullptr;
        }
    }
    inline ~File() {
        close();
    }
};
