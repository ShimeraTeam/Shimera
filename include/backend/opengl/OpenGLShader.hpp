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

#ifndef SHIMERA_OPENGLSHADER_HPP
#define SHIMERA_OPENGLSHADER_HPP

#include <string>
#include <unordered_map>

#include "backend/IShader.hpp"

namespace shimera {

/**
 * OpenGL Shader implementation using raw OpenGL shader management
 */
class OpenGLShader final : public IShader {
    public:
        OpenGLShader();
        ~OpenGLShader() override;

        OpenGLShader(const OpenGLShader&) = delete;
        OpenGLShader& operator=(const OpenGLShader&) = delete;

        void bind() const override;
        void unbind() const override;
        void setUniform(const std::string& name, const UniformValue& value) override;
        [[nodiscard]] uint32_t getNativeHandle() const override;

        void loadFromFiles(const std::string& vertPath, const std::string& fragPath);

    private:
        int getUniformLocation(const std::string& name);

        uint32_t m_programId;
        std::unordered_map<std::string, int> m_uniformCache;
};

}

#endif //SHIMERA_OPENGLSHADER_HPP
