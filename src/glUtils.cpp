// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without using any GPU knowledge
// Copyright (C) 2025-2026 The Shimera Authors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <glUtils.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

void cglClearError()
{
    while (glGetError() != GL_NO_ERROR) {};
}

bool cglLogCall(const char *function, const char *file, int line)
{
    while (const GLenum error = glGetError())
    {
        std::cerr << "[OpenGL ERROR] (" << error << "): " << function << " -> " << file << ":" << line << '\n';
        return false;
    }
    return true;
}

std::string readFile(const std::string &filePath)
{
    const std::ifstream stream(filePath);
    if (!stream.is_open()) {
        std::cerr << "ERROR: can't open file: " << filePath << '\n';
        return "";
    }
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

ShaderProgramSource parseShader(const std::string &vertexFilePath, const std::string &fragmentFilePath)
{
    ShaderProgramSource source;
    source.vertex = readFile(vertexFilePath);
    source.fragment = readFile(fragmentFilePath);
    return source;
}

unsigned int compileShader(unsigned int type, const std::string &source)
{
    GLC(const unsigned int id = glCreateShader(type);)
    const char *str = source.c_str();
    GLC(glShaderSource(id, 1, &str, nullptr));
    GLC(glCompileShader(id));

    int result;
    GLC(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int lenght;
        GLC(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
        char *message = static_cast<char *>( alloca(lenght * sizeof(char))); // -> char message[lenght]
        GLC(glGetShaderInfoLog(id, lenght, &lenght, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << '\n';
        std::cout << message << '\n';
        GLC(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int createShader(const std::string &vertexSource, const std::string &fragmentSource)
{
    GLC(const unsigned int program = glCreateProgram());
    GLC(const unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexSource));
    GLC(const unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSource));

    GLC(glAttachShader(program, vertex));
    GLC(glAttachShader(program, fragment));
    GLC(glLinkProgram(program));
    GLC(glValidateProgram(program));
    GLC(glDeleteShader(vertex));
    GLC(glDeleteShader(fragment));

    return program;
}
