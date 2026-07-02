#ifndef SHIMERA_GLUTILS_H
    #define SHIMERA_GLUTILS_H

    #include <shimera_api.h>
#include <string>

    // Cross-platform debug break
    #if defined(_WIN32) || defined(_MSC_VER)
        #define DEBUG_BREAK() __debugbreak()
    #elif defined(__GNUC__) || defined(__clang__)
        #include <csignal>
        #define DEBUG_BREAK() raise(SIGTRAP)
    #else
        #include <cstdlib>
        #define DEBUG_BREAK() abort()
    #endif

    #define ASSERT(x) if (!(x)) DEBUG_BREAK();
    #define GLC(x) cglClearError();\
    x;\
    ASSERT(cglLogCall(#x, __FILE__, __LINE__));

    namespace shimera {

    struct ShaderProgramSource {
        std::string vertex;
        std::string fragment;
    };

    }

    using ShaderProgramSource = shimera::ShaderProgramSource;

    SHIMERA_API void cglClearError();
    SHIMERA_API bool cglLogCall(const char *function, const char *file, int line);

    SHIMERA_API std::string readFile(const std::string &filePath);
    SHIMERA_API ShaderProgramSource parseShader(const std::string &vertexFilePath, const std::string &fragmentFilePath);
    SHIMERA_API unsigned int compileShader(unsigned int type, const std::string &source);
    SHIMERA_API unsigned int createShader(const std::string &vertexSource, const std::string &fragmentSource);

#endif //SHIMERA_GLUTILS_H
