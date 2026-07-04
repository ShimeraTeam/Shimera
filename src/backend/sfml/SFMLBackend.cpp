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

#include "backend/sfml/SFMLBackend.hpp"
#include "backend/sfml/SFMLFramebuffer.hpp"
#include "backend/sfml/SFMLPostProcessor.hpp"
#include "backend/sfml/SFMLShader.hpp"
#include "backend/sfml/SFMLTexture.hpp"
#include <stdexcept>

using shimera::IFrameBuffer;
using shimera::IPostProcessor;
using shimera::IShader;
using shimera::ITexture;
using shimera::SFMLBackend;
using shimera::SFMLFramebuffer;
using shimera::SFMLPostProcessor;
using shimera::SFMLShader;

IFrameBuffer* SFMLBackend::createFrameBuffer(int width, int height, bool samplableDepth) {
    return new SFMLFramebuffer(width, height, samplableDepth);
}

IPostProcessor* SFMLBackend::createPostProcessor(const std::string& vert, const std::string& frag) {
    auto *processor = new SFMLPostProcessor();
    processor->setShader(vert, frag);
    return processor;
}

ITexture* SFMLBackend::createTexture(int /*width*/, int /*height*/) {
    /** For SFML, we need to create an sf::Texture, but ITexture interface expects
    to wrap an existing texture. This would need a separate SFMLOwnedTexture class
    that owns its sf::Texture. For now it only throw an error because we don't have
    any shaders that take a texture uniform for now, so we'll see later :) */
    //TODO: Make a `SFMLOwnedTexture` class that wraps an `sf::Texture` and implements `ITexture`
    throw std::runtime_error("Direct texture creation not yet implemented for SFML backend");
}

IShader* SFMLBackend::createShader(const std::string& vert, const std::string& frag) {
    auto *shader = new SFMLShader();
    shader->loadFromFiles(vert, frag);
    return shader;
}

/** Only material available for now is FresnelEffect. It is generally only for 3D mesh.
 * So no materials for SFML for now, so no material implementation yet. **/

shimera::IMesh* SFMLBackend::createMesh(const std::vector<float>& /*positions*/,
    const std::vector<float>& /*normals*/, const std::vector<unsigned int>& /*indices*/) {
    throw std::runtime_error("Materials are not yet supported by the SFML backend");
}

shimera::IMaterial* SFMLBackend::createMaterial(const std::string& /*vert*/, const std::string& /*frag*/) {
    throw std::runtime_error("Materials are not yet supported by the SFML backend");
}

void SFMLBackend::renderMaterial(IMaterial& /*material*/, IMesh& /*mesh*/,
    const Camera& /*camera*/, const Mat4& /*transform*/) {
    throw std::runtime_error("Materials are not yet supported by the SFML backend");
}
