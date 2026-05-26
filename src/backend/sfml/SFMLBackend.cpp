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

IFrameBuffer* SFMLBackend::createFrameBuffer(int width, int height) {
    return new SFMLFramebuffer(width, height);
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
