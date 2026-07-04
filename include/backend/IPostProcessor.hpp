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

#ifndef SHIMERA_IPOSTPROCESSOR_HPP
#define SHIMERA_IPOSTPROCESSOR_HPP

#include <shimera_api.h>
#include "IShader.hpp"
#include "ITexture.hpp"
#include "uniform/Uniform.inl"

namespace shimera {

class SHIMERA_API IPostProcessor {
    public:
        struct ExtraTex {
            unsigned int unit;
            unsigned int handle;
        };

        virtual ~IPostProcessor() = default;

        virtual void setShader(const std::string& vert, const std::string& frag) = 0;
        virtual void render(ITexture& texture) = 0;
        virtual void setUniform(const std::string& name, const UniformValue& value) = 0;
        virtual void addInputTexture(const std::string& uniformName, ITexture& texture,
            unsigned int unit) = 0;
        virtual IShader& getShader() = 0;
        virtual void bindShader() = 0;
};

}

#endif //SHIMERA_IPOSTPROCESSOR_HPP