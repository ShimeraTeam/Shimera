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

#ifndef SHIMERA_SFMLBACKEND_HPP
#define SHIMERA_SFMLBACKEND_HPP

#include "backend/IBackend.hpp"

namespace shimera {

/**
 * SFML backend implementation
 * Creates SFML-specific implementations of framework interfaces
 */
class SFMLBackend final : public IBackend {
public:
    SFMLBackend() = default;
    ~SFMLBackend() override = default;

    IFrameBuffer* createFrameBuffer(int width, int height, bool samplableDepth = false) override;
    IPostProcessor* createPostProcessor(const std::string& vert, const std::string& frag) override;
    ITexture* createTexture(int width, int height) override;
    IShader* createShader(const std::string& vert, const std::string& frag) override;
    IMesh* createMesh(const std::vector<float>& positions, const std::vector<float>& normals,
        const std::vector<unsigned int>& indices) override;
    IMaterial* createMaterial(const std::string& vert, const std::string& frag) override;
    void renderMaterial(IMaterial& material, IMesh& mesh, const Camera& camera, const Mat4& transform) override;
};

}

#endif //SHIMERA_SFMLBACKEND_HPP
