#ifndef SHIMERA_SFMLFRAMEBUFFER_HPP
#define SHIMERA_SFMLFRAMEBUFFER_HPP

#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include "backend/IFrameBuffer.hpp"
#include "SFMLTexture.hpp"

/**
 * SFML Framebuffer implementation
 * Wraps around SFML RenderTexture to provide framebuffer functionality
 */
class SFMLFramebuffer final : public IFrameBuffer {
public:
    SFMLFramebuffer(int width, int height);
    ~SFMLFramebuffer() override = default;

    void bind() override;
    void unbind() override;
    void clear(Color color) override;
    ITexture& getTexture() override;
    void resize(int width, int height) override;
    int getWidth() const override;
    int getHeight() const override;
    void* getNativeRenderTarget() override;

private:
    sf::RenderTexture renderTexture;
    std::unique_ptr<SFMLTexture> texture;
    int m_width, m_height;
};

#endif //SHIMERA_SFMLFRAMEBUFFER_HPP