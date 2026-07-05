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

#ifndef SHIMERA_OPENGLMESH_HPP
#define SHIMERA_OPENGLMESH_HPP

#include <vector>

#include "shimera_api.h"
#include "backend/IMesh.hpp"

namespace shimera {

class SHIMERA_API OpenGLMesh final : public IMesh {
    public:
        OpenGLMesh(const std::vector<float>& positions,
            const std::vector<float>& normals,
            const std::vector<unsigned int>& indices);
        ~OpenGLMesh() override;

        OpenGLMesh(const OpenGLMesh&) = delete;
        OpenGLMesh& operator=(const OpenGLMesh&) = delete;

        void draw() const;
        [[nodiscard]] unsigned int getIndexCount() const;

    private:
        unsigned int m_vao = 0;
        unsigned int m_vboPos = 0;
        unsigned int m_vboNormal = 0;
        unsigned int m_ebo = 0;
        unsigned int m_indexCount = 0;
};

}

#endif //SHIMERA_OPENGLMESH_HPP
