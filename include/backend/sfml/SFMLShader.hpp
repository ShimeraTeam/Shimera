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

#ifndef SHIMERA_SFMLSHADER_HPP
#define SHIMERA_SFMLSHADER_HPP

#include "backend/IShader.hpp"
#include <string>
#include <unordered_map>

namespace shimera {

/**
 * SFML Shader implementation using OpenGL
 * Since SFML uses OpenGL underneath, we use raw OpenGL shader management
 */
//TODO: Maybe later try to share the same code between SFML and OpenGL backends, maybe...
class SFMLShader final : public IShader {
    public:
        SFMLShader();
        ~SFMLShader() override;

        // Prevent copying
        SFMLShader(const SFMLShader&) = delete;
        SFMLShader& operator=(const SFMLShader&) = delete;

        void bind() const override;
        void unbind() const override;
        void setUniform(const std::string& name, const UniformValue& value) override;
        uint32_t getNativeHandle() const override;

        void loadFromFiles(const std::string& vertPath, const std::string& fragPath);

    private:
        int getUniformLocation(const std::string& name);
    
        uint32_t m_programId;
        // Uniform cache to lower the CPU and GPU communications
        std::unordered_map<std::string, int> m_uniformCache;
};

}

#endif //SHIMERA_SFMLSHADER_HPP
