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
#include <vector>

namespace shimera {

/**
 * Indexed triangle geometry: positions, normals, indices.
 *
 * Anything host-specific is not here. Mesh data arrives as plain float vectors,
 * so the same class serves a raylib user, an SFML user, or someone who parsed an .obj
 * themselves.
 */
class GLMesh {
    public:
        GLMesh(const std::vector<float>& positions,
               const std::vector<float>& normals,
               const std::vector<unsigned int>& indices);
        ~GLMesh();

        GLMesh(const GLMesh&) = delete;
        GLMesh& operator=(const GLMesh&) = delete;
        GLMesh(GLMesh&&) = delete;
        GLMesh& operator=(GLMesh&&) = delete;

        // Binds the VAO and draws. Leaves it bound and GLStateGuard puts it back.
        void draw() const;

        [[nodiscard]] unsigned int getIndexCount() const { return m_indexCount; }

    private:
        uint32_t m_vao = 0;
        uint32_t m_positionBuffer = 0;
        uint32_t m_normalBuffer = 0;
        uint32_t m_indexBuffer = 0;
        unsigned int m_indexCount = 0;
};

}
