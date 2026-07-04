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

#include <stdexcept>

#include "backend/raylib/converts/RaylibConvert.hpp"
#include "backend/raylib/RaylibFramebuffer.hpp"
#include "backend/raylib/RaylibMaterial.hpp"
#include "backend/raylib/RaylibMesh.hpp"
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

shimera::IMesh* RaylibBackend::createMesh(const std::vector<float>&  /*positions*/,
    const std::vector<float>&  /*normals*/, const std::vector<unsigned int>&  /*indices*/) {
    throw std::runtime_error("Not supported by this backend, use the backend's native mesh type instead ");
}

shimera::IMaterial* RaylibBackend::createMaterial(const std::string& vert, const std::string& frag) {
    return new RaylibMaterial(vert, frag);
}

void RaylibBackend::renderMaterial(IMaterial& material, IMesh& mesh,
    const Camera& cam, const Mat4& transform) {
    auto& mat = static_cast<RaylibMaterial&>(material);
    auto& rmesh = static_cast<RaylibMesh&>(mesh);
    Model& model = rmesh.model();

    mat.setUniform("u_cameraPos", cam.position);

    const Shader previous = model.materials[0].shader;
    model.materials[0].shader = mat.shader();
    model.transform = toRaylibMatrix(transform);
    DrawModel(model, Vector3{0, 0, 0}, 1.0f, {255, 255, 255, 255});
    model.materials[0].shader = previous;
}

ITexture* RaylibBackend::createTexture(int /*width*/, int /*height*/) {
    return nullptr;
}

IShader* RaylibBackend::createShader(const std::string& vert, const std::string& frag) {
    auto *shader = new RaylibShader();
    shader->loadFromFiles(vert, frag);
    return shader;
}
