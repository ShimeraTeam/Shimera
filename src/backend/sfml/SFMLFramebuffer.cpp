#include "backend/sfml/SFMLFramebuffer.hpp"
#include <stdexcept>
#include <string>
#include <cstdint>

using shimera::ITexture;
using shimera::SFMLFramebuffer;
using shimera::SFMLTexture;

SFMLFramebuffer::SFMLFramebuffer(const int width, const int height)
    : m_width(width), m_height(height) {
    SFMLFramebuffer::resize(width, height);
}

void SFMLFramebuffer::bind() {
    m_renderTexture.setActive(true);
}

void SFMLFramebuffer::unbind() {
    m_renderTexture.display();
    m_renderTexture.setActive(false);
}

void SFMLFramebuffer::clear(shimera::Color color) {
    sf::Color sfmlColor(
        static_cast<std::uint8_t>(color.m_r * 255),
        static_cast<std::uint8_t>(color.m_g * 255),
        static_cast<std::uint8_t>(color.m_b * 255),
        static_cast<std::uint8_t>(color.m_a * 255)
    );
    m_renderTexture.clear(sfmlColor);
}

ITexture& SFMLFramebuffer::getTexture() {
    return *m_texture;
}

void SFMLFramebuffer::resize(int width, int height) {
    m_width = width;
    m_height = height;
    
    if (!m_renderTexture.resize({static_cast<unsigned>(width), static_cast<unsigned>(height)})) {
        throw std::runtime_error("Failed to resize SFML framebuffer to " + 
                                 std::to_string(width) + "x" + std::to_string(height));
    }
    m_texture = std::make_unique<SFMLTexture>(m_renderTexture.getTexture());
}

int SFMLFramebuffer::getWidth() const {
    return m_width;
}

int SFMLFramebuffer::getHeight() const {
    return m_height;
}

void* SFMLFramebuffer::getNativeRenderTarget() {
    return &m_renderTexture;
}
