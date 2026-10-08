#pragma once

#if defined(SHIMERA_STATIC)
    #define EXPORT
#elif defined(_WIN32)
    #if defined(SHIMERA_EXPORTS)
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT __declspec(dllimport)
    #endif
#elif defined(__GNUC__) || defined(__clang__)
    #define EXPORT __attribute__((visibility("default")))
#else
    #define EXPORT
#endif
