#include <stdio.h>

#if defined(_WIN32)
// let user include windows.h for us because it has many defined that users often want to control

// should be harmless

#define _NO_OLDNAMES   // for MinGW
#include <conio.h>     // getch and kbhit

#elif defined(__linux__)
#include <sys/ioctl.h>

// System includes, mostly kbhit emulation
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>

// GetCH emulation + time
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#else
#error unsupported platform, we support windows via _WIN32 and linux via __linux__
#endif

#include <stdint.h>
#include <deque>

struct MiniEdit {
    // buffer for non-blocking io, unix and windows
    std::deque<int> buffer;

    // the number of pending inputs to read
    uint64_t pending = 0;

    int in_fd;
    int out_fd;

    // https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
    // this applies to ANSI as well

    inline MiniEdit() : MiniEdit(fd_stdin(), fd_stdout()) {}
    inline MiniEdit(int in_fd, int out_fd) : in_fd(in_fd), out_fd(out_fd) {}

#if defined(_WIN32)
    inline static int fd_stdin() { return _fileno(stdin); }
    inline static int fd_stdout() { return _fileno(stdout); }
    inline static int fd_stderr() { return _fileno(stderr); }
    inline static bool is_a_tty(int fd) { return _isatty(fd); }
    inline static void terminal_size(int fd, int * width, int * height) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(fd), &csbi);
        width = (int)(csbi.srWindow.Right-csbi.srWindow.Left+1);
        height = (int)(csbi.srWindow.Bottom-csbi.srWindow.Top+1);
    }

    private:
    struct RawInputScope {
        DWORD old_save;
        HANDLE handle;

        inline RawInputScope() : RawInputScope(fd_stdin()) {}

        inline RawInputScope(int fd) : handle(_get_osfhandle(fd)) {
            // Set up the console.
            if (handle == INVALID_HANDLE_VALUE) return;
            if (!GetConsoleMode(handle, &old_save)) return;

            // https://learn.microsoft.com/en-us/windows/console/ctrl-c-and-ctrl-break-signals
            // in case we need to implement debugging (CTRL+C kills child instead of us)
            // The SetConsoleMode function can disable the ENABLE_PROCESSED_INPUT input mode for a console's input buffer,
            // so CTRL+C is reported as keyboard input rather than as a signal.

            DWORD new_save = old_save
                | ENABLE_VIRTUAL_TERMINAL_INPUT
                | ENABLE_PROCESSED_INPUT; // we want system to handle ^C for program termination
            new_save = (old_save & ~(
                ENABLE_ECHO_INPUT           // dont want echo
                | ENABLE_INSERT_MODE        // dont want insert
                | ENABLE_LINE_INPUT         // dont want buffered input
                | ENABLE_MOUSE_INPUT        // dont want mouse events
                | ENABLE_WINDOW_INPUT       // dont want resize events
            ));
            SetConsoleMode(handle, &new_save);
        }

        inline ~RawInputScope() {
            // restore console
            SetConsoleMode(handle, old_save);
        }
    };

    struct RawOutputScope {
        DWORD old_save;
        UINT old_codepage;
        char * old_locale;
        HANDLE handle;

        inline RawOutputScope() : RawOutputScope(fd_stdout()) {}

        inline RawOutputScope(int fd) : handle(_get_osfhandle(fd)) {
            // Set up the console.
            if (handle == INVALID_HANDLE_VALUE) return;
            if (!GetConsoleMode(handle, &old_save)) return;
            new_save = old_save | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
            if (!SetConsoleMode(handle, new_save)) {
                new_save = old_save | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(handle, new_save);
            }
            old_codepage = GetConsoleCP();
            SetConsoleCP( 65001 );
            old_locale = setlocale( LC_ALL, ".UTF-8" );
        }

        inline ~RawOutputScope() {
            // restore console
            setlocale( LC_ALL, old_locale );
            SetConsoleCP(old_codepage);
            SetConsoleMode(handle, old_save);
        }
    };

    public:

    inline int read_pending(HANDLE handle) {
        // https://stackoverflow.com/questions/24708700/c-detect-when-user-presses-arrow-key/24709138#24709138

        while(pending != 0) {
            INPUT_RECORD rec;
            DWORD ret;
            if (!ReadConsoleInput(handle, rec, 1, &ret)) break;
            if (ret == 0) {
                // we have pending input but we failed to read for some reason
                break;
            }
            if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown) {
                // windows maps keys to virtual keys
                // we need to translate the virtual keys into ASCII sequences
                // rec.Event.KeyEvent.wVirtualKeyCode == VK_LEFT
                // generate \033 [ A

                // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

                // In general, virtual-key codes do not correspond to ASCII codes or any other character-encoding standard
                // No constants are defined for the virtual-key codes that match ASCII values.
                // For example, the virtual-key code for the A key is 0x41, but there is no constant named VK_A
                if (rec.Event.KeyEvent.wVirtualKeyCode >= 0x30 && rec.Event.KeyEvent.wVirtualKeyCode <= 0x5A) {
                    // a-z, A-Z, 0-9
                    buffer.push_front((int)rec.Event.KeyEvent.wVirtualKeyCode);
                } else {
                    // decode keycode
                    //
                    // NOTE: pressing ctrl by itself will not generate a VK_*CONTROL event for `ReadConsoleInput`
                    //       pressing ctrl by itself will     generate a VK_*CONTROL event for `getKeyState`
                    //
                    // https://stackoverflow.com/questions/27586641/missing-events-on-readconsoleinput-in-windows-shell/68237551#68237551
                    // confusing
                    //
                    switch(rec.Event.KeyEvent.wVirtualKeyCode) {
                        // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
                        // case 
                        default:
                            printf("unknown virtual keycode: %d\n\r", rec.Event.KeyEvent.wVirtualKeyCode);
                    }
                }
            }
            pending--;
        }
        return buffer.size();
    }

    inline int has_input(int fd, int timeout_ms) {
        RawInputScope ti(fd);
        
        HANDLE handle = _get_osfhandle(fd);
        if (handle == INVALID_HANDLE_VALUE) return buffer.size();
        // https://learn.microsoft.com/en-us/windows/console/reading-input-buffer-events

        DWORD recn;
        if (!GetNumberOfConsoleInputEvents(handle, &recn)) return buffer.size();
        if (recn == 0) {
            // wait if we have no events
            // we assume that `SetConsoleMode` with no mouse/window events will not cause
            // WaitForSingleObject to NOT return if such events end up in the queue somehow
            DWORD ret = WaitForSingleObject(handle, (DWORD)timeout_ms);
            if (ret != WAIT_OBJECT_0) return buffer.size();
            // make sure we have events
            if (!GetNumberOfConsoleInputEvents(handle, &recn)) return buffer.size();
            if (recn == 0) return buffer.size();
        }
        // we have events, read them
        pending += recn;
        return read_pending(handle);
    }
#elif defined(__linux__)
    inline static int fd_stdin() { return fileno(stdin); }
    inline static int fd_stdout() { return fileno(stdout); }
    inline static int fd_stderr() { return fileno(stderr); }
    inline static bool is_a_tty(int fd) { return isatty(fd); }
    inline static void terminal_size(int fd, int * width, int * height) {
        struct winsize w;
        ioctl(fd, TIOCGWINSZ, &w);
        *width = (int)(w.ws_col);
        *height = (int)(w.ws_row);
    }

    private:
    struct RawInputScope {
        struct termios oldTermios; // Save off for previous terminal settings
        int fd;

        inline RawInputScope() : RawInputScope(fd_stdin()) {}

        inline RawInputScope(int fd) : fd(fd) {
            // Set up the console.
            tcgetattr(fd, &oldTermios);                                     // Get old settings
            struct termios newTermios = oldTermios;                         // copy old settings to new settings
            newTermios.c_oflag = 0;                                         // Output mode
            newTermios.c_lflag &= ~(ICANON | ECHO);                         // unset canonical mode, unset echo.
            newTermios.c_iflag = 0;                                         // Input mode
            newTermios.c_cc[VMIN] = 1;                                      // Minimum time to wait
            newTermios.c_cc[VTIME] = 1;                                     // Minimum characters to wait for
            tcsetattr(fd, TCSANOW, &newTermios);                            // Set newTermios
        }

        inline ~RawInputScope() {
            // restore console
            tcsetattr(fd, TCSANOW, &oldTermios);
        }
    };

    struct RawOutputScope {
        // unix terminals are ANSI by default, but may be non-utf locale
        char * old_locale;

        inline RawOutputScope() : RawOutputScope(fd_stdout()) {}

        inline RawOutputScope(int fd) { old_locale = setlocale( LC_ALL, ".UTF-8" ); }

        inline ~RawOutputScope() { setlocale( LC_ALL, old_locale ); }
    };
    
    public:

    inline int read_pending(int fd) {
        while(pending != 0) {
            int buf;
            int ret = read(fd, &buf, 1);
            if (ret == -1) {
                if (errno == EINTR) continue;
                break;
            }
            if (ret == 0) {
                // we have pending input but we failed to read for some reason
                break;
            }
            // we read some input
            buffer.push_front(buf);
            pending--;
        }
        return buffer.size();
    }

    // termios reference: http://man7.org/linux/man-pages/man3/termios.3.html
    // More readable termios reference: https://www.mkssoftware.com/docs/man5/struct_termios.5.asp
    // select reference: http://man7.org/linux/man-pages/man2/select.2.html
    inline int has_input(int fd, int timeout_ms) {
        RawInputScope ti(fd);

        int charCount = 0;

        if (ioctl(fd, FIONREAD, &charCount) == -1) return buffer.size();
        if (charCount == 0) {
            // wait if we have no input

            struct timeval tv;
            if (timeout_ms >= 1000) {
                tv.tv_sec = timeout_ms % 1000;          // time to delay, in Seconds.
                tv.tv_usec = timeout_ms - tv.tv_sec;    // time to delay, in Microseconds
            } else {
                tv.tv_sec = 0;              // time to delay, in Seconds.
                tv.tv_usec = timeout_ms;    // time to delay, in Microseconds
            }

            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(fd, &rfds);
            while (true) {
                int ret = select(1, &rfds, NULL, NULL, &tv);
                if (ret == -1) {
                    if (errno == EINTR) continue;
                    return buffer.size();
                }
                break;
            }
            // ensure we have input
            if (ioctl(fd, FIONREAD, &charCount) == -1) return buffer.size();
            if (charCount == 0) return buffer.size();
        }
        // we have input
        pending += charCount;
        return read_pending(fd);
    }
#endif

    inline int has_input(int fd) { return has_input(fd, 100); }
    inline int get_char(int fd) {
        if (buffer.size() == 0) return EOF;
        int ch = buffer.back();
        buffer.pop_back();
        return ch;

    }

    inline void loop() {
        while (true)
        {
            int num = has_input(fd_stdin(), 1000);
            if (num > 0)
            {
                printf("obtained input...\n");
                // Count down to handle wide characters, or multi-character input.
                while (num--)
                {
                    int lastCode = get_char(fd_stdin());
                    printf("keycode: %d x%02X, char: %c\n", lastCode, lastCode, (char)lastCode);
                    if (lastCode == 0x0 || lastCode == 0x4) return;
                }
                printf("handled input, press CTRL+D or CTRL+SPACE to exit loop\n");
            }
        }
    }

    // Orig (0;0) -> (top;left)

    inline void save() { RawOutputScope o(out_fd); printf("\0337"); }
    inline void restore() { RawOutputScope o(out_fd); printf("\0338"); }
    inline void move_up(uint16_t relative) { RawOutputScope o(out_fd); printf("\033[%dA", relative); }
    inline void move_down(uint16_t relative) { RawOutputScope o(out_fd); printf("\033[%dB", relative); }
    inline void move_left(uint16_t relative) { RawOutputScope o(out_fd); printf("\033[%dD", relative); }
    inline void move_right(uint16_t relative) { RawOutputScope o(out_fd); printf("\033[%dC", relative); }
    inline void move_to(uint16_t x, uint16_t y) { RawOutputScope o(out_fd); printf("\033[%d,%dH", x, y); }
    inline void move_newline() { RawOutputScope o(out_fd); printf("\033[E"); }
    inline void move_line_start() { RawOutputScope o(out_fd); printf("\r"); }
    inline void delete_line(void) { RawOutputScope o(out_fd); printf("\033[2K"); }
    inline void delete_contents_after_cursor(void) { RawOutputScope o(out_fd); printf("\033[K"); }
};
