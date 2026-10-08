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

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "GLShader.hpp"
#include "GLTexture.hpp"

namespace shimera {

/**
 * One fullscreen-quad draw through one shader.
 *
 * render() binds shader, VAO and textures, draws, and stops. It deliberately does not
 * unbind anything afterwards. See GLTexture for why undoing state by zeroing it is
 * worse than leaving it to GLStateGuard.
 */
class GLPostProcessor {
    public:
        GLPostProcessor(const std::string& vertPath, const std::string& fragPath);
        ~GLPostProcessor();

        GLPostProcessor(const GLPostProcessor&) = delete;
        GLPostProcessor& operator=(const GLPostProcessor&) = delete;
        GLPostProcessor(GLPostProcessor&&) = delete;
        GLPostProcessor& operator=(GLPostProcessor&&) = delete;

        // Draws the quad, sampling `input` from texture unit 0.
        void render(const GLTexture& input);

        /**
         * Binds an extra texture for the NEXT render() only, and points `uniformName`
         * at its unit. Cleared after each draw, so effects re-declare their inputs every
         * frame, which is what makes a resized intermediate buffer safe.
         */
        void addInputTexture(const std::string& uniformName, const GLTexture& texture, int unit);

        void setUniform(const std::string& name, const UniformValue& value);

        [[nodiscard]] GLShader& shader() const { return *m_shader; }

    private:
        void initializeQuad();

        struct ExtraTexture {
            int unit;
            uint32_t handle;
        };

        uint32_t m_vao = 0;
        uint32_t m_vbo = 0;
        uint32_t m_ebo = 0;
        std::unique_ptr<GLShader> m_shader;
        std::vector<ExtraTexture> m_extraTextures;
};

}
