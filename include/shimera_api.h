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

#ifndef SHIMERA_API_H
#define SHIMERA_API_H

//TODO: Rename `SHIMERA_API` to `EXPOSE`

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
