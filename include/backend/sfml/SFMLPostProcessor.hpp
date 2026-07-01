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

#ifndef SHIMERA_SFMLPOSTPROCESSOR_HPP
#define SHIMERA_SFMLPOSTPROCESSOR_HPP

#include "backend/IPostProcessor.hpp"
#include "backend/IShader.hpp"
#include <vector>
#include <memory>

namespace shimera {

class SFMLPostProcessor final : public IPostProcessor {
    public:
        SFMLPostProcessor();
        ~SFMLPostProcessor() override;

        // Prevent copying
        SFMLPostProcessor(const SFMLPostProcessor&) = delete;
        SFMLPostProcessor& operator=(const SFMLPostProcessor&) = delete;

        void setShader(const std::string& vert, const std::string& frag) override;
        void render(ITexture& texture) override;
        void setUniform(const std::string& name, const UniformValue& value) override;
        void addInputTexture(const std::string& uniformName, ITexture& texture, unsigned int unit) override;
        IShader& getShader() override;
        void bindShader() override;

    private:
        std::vector<ExtraTex> m_extraTextures;

        void initializeQuad();
        void cleanupQuad();
        void cleanupShader();

        unsigned int m_vao;
        unsigned int m_vbo;
        unsigned int m_ebo;

        std::unique_ptr<IShader> m_shader;
};

}

#endif //SHIMERA_SFMLPOSTPROCESSOR_HPP
