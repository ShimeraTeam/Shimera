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

#ifndef SHIMERA_OPENGLBACKEND_HPP
#define SHIMERA_OPENGLBACKEND_HPP

#include "backend/IBackend.hpp"

namespace shimera {

/**
 * OpenGL backend implementation
 * Creates OpenGL-specific implementations of framework interfaces
 */
class OpenGLBackend final : public IBackend {
    public:
        OpenGLBackend() = default;
        ~OpenGLBackend() override = default;

        IFrameBuffer* createFrameBuffer(int width, int height, bool samplableDepth = false) override;
        IPostProcessor* createPostProcessor(const std::string& vert, const std::string& frag) override;
        ITexture* createTexture(int width, int height) override;
        IShader* createShader(const std::string& vert, const std::string& frag) override;
};

}

#endif //SHIMERA_OPENGLBACKEND_HPP
