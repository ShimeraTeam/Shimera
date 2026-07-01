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

#ifndef OPENGL_LEARNING_UTILS_H
    #define OPENGL_LEARNING_UTILS_H

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

#endif //OPENGL_LEARNING_UTILS_H
