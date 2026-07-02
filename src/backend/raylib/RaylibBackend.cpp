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
