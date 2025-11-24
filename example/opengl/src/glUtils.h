//
// Created by leoma on 24/11/2025.
//

#ifndef OPENGL_LEARNING_UTILS_H
    #define OPENGL_LEARNING_UTILS_H

    #include <string>

    #define ASSERT(x) if (!(x)) __debugbreak();
    #define GLC(x) cglClearError();\
    x;\
    ASSERT(cglLogCall(#x, __FILE__, __LINE__));

    struct ShaderProgramSource
    {
        std::string vertex;
        std::string fragment;
    };

    void cglClearError();
    bool cglLogCall(const char *function, const char *file, int line);

    std::string readFile(const std::string &filePath);
    ShaderProgramSource parseShader(const std::string &vertexFilePath, const std::string &fragmentFilePath);
    unsigned int compileShader(unsigned int type, const std::string &source);
    unsigned int createShader(const std::string &vertexSource, const std::string &fragmentSource);

#endif //OPENGL_LEARNING_UTILS_H