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
