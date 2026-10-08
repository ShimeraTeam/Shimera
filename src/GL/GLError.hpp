// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without any GPU knowledge
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
