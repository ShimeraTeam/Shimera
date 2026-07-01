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

#ifndef SHIMERA_ITEXTURE_HPP
#define SHIMERA_ITEXTURE_HPP

#include <shimera_api.h>

namespace shimera {

class SHIMERA_API ITexture {
    public:
        virtual ~ITexture() = default;

        virtual void bind(int slot = 0) = 0;
        virtual void unbind() = 0;
        [[nodiscard]] virtual unsigned int getNativeHandle() const = 0;
        [[nodiscard]] virtual int getWidth() const = 0;
        [[nodiscard]] virtual int getHeight() const = 0;
};

}

#endif //SHIMERA_ITEXTURE_HPP