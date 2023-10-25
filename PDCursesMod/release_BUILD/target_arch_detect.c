

    #if defined(_M_ARM) || defined(__arm__)
        #error cmake_arch ARM
    #elif defined(_M_ARM64) || defined(__aarch64__)
        #error cmake_arch ARM64
    #elif defined(_M_AMD64) || defined(__x86_64__)
        #error cmake_arch x86_64
    #elif defined(_M_X64)
        #error cmake_arch x64
    #elif defined(_M_IX86) || defined(__i386__)
        #error cmake_arch x86
    #else
        #error cmake_arch unknown
    #endif
