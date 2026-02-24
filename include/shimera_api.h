#ifndef SHIMERA_API_H
#define SHIMERA_API_H

// DLL export/import macros for Windows
#if defined(_WIN32) || defined(_MSC_VER)
    #ifdef SHIMERA_EXPORTS
        #define SHIMERA_API __declspec(dllexport)
    #elif defined(SHIMERA_SHARED)
        #define SHIMERA_API __declspec(dllimport)
    #else
        #define SHIMERA_API
    #endif
#else
    // Non-Windows platforms
    #if defined(__GNUC__) && __GNUC__ >= 4
        #define SHIMERA_API __attribute__((visibility("default")))
    #else
        #define SHIMERA_API
    #endif
#endif

#endif // SHIMERA_API_H
