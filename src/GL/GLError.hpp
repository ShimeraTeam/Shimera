#pragma once

#include <shimera_api_export.h>

/*
 * GL error checking.
 */

#if defined(_WIN32) || defined(_MSC_VER)
    #define SHIMERA_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #include <csignal>
    #define SHIMERA_DEBUG_BREAK() raise(SIGTRAP)
#else
    #include <cstdlib>
    #define SHIMERA_DEBUG_BREAK() abort()
#endif

// Prefixed because the old names (ASSERT, DEBUG_BREAK) are unqualified macros in a header a user includes.
#define SHIMERA_ASSERT(x) if (!(x)) SHIMERA_DEBUG_BREAK();

#define GLC(x) ::shimera::clearGLErrors();\
x;\
SHIMERA_ASSERT(::shimera::logGLCall(#x, __FILE__, __LINE__));

namespace shimera {

// Drains the GL error queue so the next logGLCall() reports only what x itself caused.
EXPORT void clearGLErrors();

// Returns false and writes to stderr if the queue is non-empty.
EXPORT bool logGLCall(const char* function, const char* file, int line);

} // namespace shimera
