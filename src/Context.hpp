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

#include <memory>
#include <string>

namespace shimera {

/**
 * Owns library-wide GL setup. Create it ONCE, after the host window exists.
 *
 * Loading GL function pointers requires a current context, and only the host can make one.
 * So create() detects the mistake and throws a message naming the call they are missing, per library.
 */
class Context {
    public:
        // Throws shimera::InitError if there is no current context, or GL cannot load.
        static Context create();

        ~Context();
        Context(Context&&) noexcept;
        Context& operator=(Context&&) noexcept;
        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

        [[nodiscard]] static std::string getRendererInfo() ;

    private:
        Context();
};

}
