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

#include "backend/opengl/OpenGLBackend.hpp"
#include "backend/opengl/OpenGLFramebuffer.hpp"
#include "backend/opengl/OpenGLPostProcessor.hpp"
#include "backend/opengl/OpenGLShader.hpp"
#include "backend/opengl/OpenGLTexture.hpp"

#include <stdexcept>

using shimera::IFrameBuffer;
using shimera::IPostProcessor;
using shimera::IShader;
using shimera::ITexture;
using shimera::OpenGLBackend;
using shimera::OpenGLFramebuffer;
using shimera::OpenGLPostProcessor;
using shimera::OpenGLShader;
using shimera::OpenGLTexture;

IFrameBuffer* OpenGLBackend::createFrameBuffer(int width, int height, bool samplableDepth) {
    return new OpenGLFramebuffer(width, height, samplableDepth);
}

IPostProcessor* OpenGLBackend::createPostProcessor(const std::string& vert, const std::string& frag) {
    auto *processor = new OpenGLPostProcessor();
    processor->setShader(vert, frag);
    return processor;
}

ITexture* OpenGLBackend::createTexture(const int width, const int height) {
    return new OpenGLTexture(width, height);
}

IShader* OpenGLBackend::createShader(const std::string& vert, const std::string& frag) {
    auto *shader = new OpenGLShader();
    shader->loadFromFiles(vert, frag);
    return shader;
}
