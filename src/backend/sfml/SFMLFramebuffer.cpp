#include "backend/sfml/SFMLFramebuffer.hpp"
#include <stdexcept>
#include <string>
#include <cstdint>

SFMLFramebuffer::SFMLFramebuffer(const int width, const int height)
    : m_width(width), m_height(height) {
    SFMLFramebuffer::resize(width, height);
}

void SFMLFramebuffer::bind() {
    renderTexture.setActive(true);
}

void SFMLFramebuffer::unbind() {
    renderTexture.display();
    renderTexture.setActive(false);
}

void SFMLFramebuffer::clear(Color color) {
    sf::Color sfmlColor(
        static_cast<std::uint8_t>(color.r * 255),
        static_cast<std::uint8_t>(color.g * 255),
        static_cast<std::uint8_t>(color.b * 255),
        static_cast<std::uint8_t>(color.a * 255)
    );
    renderTexture.clear(sfmlColor);
}

ITexture& SFMLFramebuffer::getTexture() {
    return *texture;
}

void SFMLFramebuffer::resize(int width, int height) {
    m_width = width;
    m_height = height;
    
    if (!renderTexture.resize({static_cast<unsigned>(width), static_cast<unsigned>(height)})) {
        throw std::runtime_error("Failed to resize SFML framebuffer to " + 
                                 std::to_string(width) + "x" + std::to_string(height));
    }
    texture = std::make_unique<SFMLTexture>(renderTexture.getTexture());
}

int SFMLFramebuffer::getWidth() const {
    return m_width;
}

int SFMLFramebuffer::getHeight() const {
    return m_height;
}

void* SFMLFramebuffer::getNativeRenderTarget() {
    return &renderTexture;
}
