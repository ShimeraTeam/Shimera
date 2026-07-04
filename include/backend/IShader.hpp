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

#ifndef SHIMERA_ISHADER_HPP
#define SHIMERA_ISHADER_HPP

#include <shimera_api.h>
#include <cstdint>
#include <string>

#include "uniform/Uniform.inl"

namespace shimera {

class SHIMERA_API IShader {
    public:
        virtual ~IShader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void setUniform(const std::string& name, const UniformValue& value) = 0;
        [[nodiscard]] virtual uint32_t getNativeHandle() const = 0;
};

}

#endif //SHIMERA_ISHADER_HPP