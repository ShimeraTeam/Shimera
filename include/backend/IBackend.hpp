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

#ifndef SHIMERA_IBACKEND_HPP
#define SHIMERA_IBACKEND_HPP

#include <shimera_api.h>
#include <vector>

#include "IFrameBuffer.hpp"
#include "IMaterial.hpp"
#include "IPostProcessor.hpp"
#include "IMesh.hpp"
#include "scene/Camera.hpp"

namespace shimera {

/**
 * Backend interface
 * Defines methods that should be implemented by any rendering backend and serves as a bridge
 */
class SHIMERA_API IBackend {
    public:
        virtual ~IBackend() = default;

        virtual IFrameBuffer* createFrameBuffer(int width, int height,
                                                    bool samplableDepth = false) = 0;
        virtual IPostProcessor* createPostProcessor(const std::string& vert,
                                                        const std::string& frag) = 0;
        virtual IMesh* createMesh(const std::vector<float>& positions,
                          const std::vector<float>& normals,
                          const std::vector<unsigned int>& indices) = 0;
        virtual IMaterial* createMaterial(const std::string& vert,
                                          const std::string& frag) = 0;
        virtual void renderMaterial(IMaterial& material, IMesh& mesh,
                                    const Camera& camera, const Mat4& transform) = 0;
        virtual ITexture* createTexture(int width, int height) = 0;
        virtual IShader* createShader(const std::string& vert,
                                        const std::string& frag) = 0;
};

}

#endif //SHIMERA_IBACKEND_HPP