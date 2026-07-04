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

#ifndef SHIMERA_RAYLIBSHADER_HPP
#define SHIMERA_RAYLIBSHADER_HPP

#include "backend/IShader.hpp"
#include <string>
#include <unordered_map>

namespace shimera {

/**
 * Raylib Shader implementation using OpenGL
 * Since Raylib uses OpenGL underneath, we use raw OpenGL shader management
 */
//TODO: Maybe later try to share the same code between SFML, OpenGL and Raylib backends, maybe...
class RaylibShader final : public IShader {
    public:
        RaylibShader();
        ~RaylibShader() override;

        // Prevent copying
        RaylibShader(const RaylibShader&) = delete;
        RaylibShader& operator=(const RaylibShader&) = delete;

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

#endif //SHIMERA_RAYLIBSHADER_HPP
