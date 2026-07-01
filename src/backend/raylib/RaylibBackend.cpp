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

#include "backend/raylib/RaylibBackend.hpp"
#include "backend/raylib/RaylibFramebuffer.hpp"
#include "backend/raylib/RaylibPostProcessor.hpp"
#include "backend/raylib/RaylibShader.hpp"

using shimera::IFrameBuffer;
using shimera::IPostProcessor;
using shimera::IShader;
using shimera::ITexture;
using shimera::RaylibBackend;
using shimera::RaylibFramebuffer;
using shimera::RaylibPostProcessor;
using shimera::RaylibShader;

IFrameBuffer* RaylibBackend::createFrameBuffer(int width, int height, bool samplableDepth) {
    return new RaylibFramebuffer(width, height, samplableDepth);
}

IPostProcessor* RaylibBackend::createPostProcessor(const std::string& vert, const std::string& frag) {
    auto *processor = new RaylibPostProcessor();
    processor->setShader(vert, frag);
    return processor;
}

ITexture* RaylibBackend::createTexture(int /*width*/, int /*height*/) {
    return nullptr;
}

IShader* RaylibBackend::createShader(const std::string& vert, const std::string& frag) {
    auto *shader = new RaylibShader();
    shader->loadFromFiles(vert, frag);
    return shader;
}
