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
