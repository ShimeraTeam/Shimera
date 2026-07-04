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

#include <GL/glew.h>

#include "backend/opengl/OpenGLFramebuffer.hpp"
#include "backend/opengl/OpenGLPostProcessor.hpp"
#include "backend/opengl/OpenGLShader.hpp"
#include "backend/opengl/OpenGLTexture.hpp"
#include "backend/opengl/OpenGLMaterial.hpp"
#include "backend/opengl/OpenGLMesh.hpp"

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

shimera::IMesh* OpenGLBackend::createMesh(const std::vector<float>& positions, const std::vector<float>& normals,
    const std::vector<unsigned int>& indices) {
    return new OpenGLMesh(positions, normals, indices);
}

shimera::IMaterial* OpenGLBackend::createMaterial(const std::string& vert, const std::string& frag) {
    return new OpenGLMaterial(vert, frag);
}

void OpenGLBackend::renderMaterial(IMaterial& material, IMesh& mesh, const Camera& camera, const Mat4& transform) {
    auto& mat = static_cast<OpenGLMaterial&>(material);
    auto& glm = static_cast<OpenGLMesh&>(mesh);
    mat.setUniform("u_model", transform);
    mat.setUniform("u_view", camera.view);
    mat.setUniform("u_projection", camera.projection);
    mat.setUniform("u_cameraPos", camera.position);
    glEnable(GL_DEPTH_TEST);
    mat.shader().bind();
    glm.draw();
    mat.shader().unbind();
}

ITexture* OpenGLBackend::createTexture(const int width, const int height) {
    return new OpenGLTexture(width, height);
}

IShader* OpenGLBackend::createShader(const std::string& vert, const std::string& frag) {
    auto *shader = new OpenGLShader();
    shader->loadFromFiles(vert, frag);
    return shader;
}
