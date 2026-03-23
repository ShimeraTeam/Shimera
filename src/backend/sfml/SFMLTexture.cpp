#include <GL/glew.h>
#include <glUtils.h>
#include <SFML/Graphics/Texture.hpp>
#include "backend/sfml/SFMLTexture.hpp"

SFMLTexture::SFMLTexture(const sf::Texture& texture) : m_texture(texture) {}

void SFMLTexture::bind(const int slot) {
    GLC(glActiveTexture(GL_TEXTURE0 + slot));
    GLC(glBindTexture(GL_TEXTURE_2D, m_texture.getNativeHandle()));
}

void SFMLTexture::unbind() {
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int SFMLTexture::getNativeHandle() const {
    return m_texture.getNativeHandle();
}

int SFMLTexture::getWidth() const {
    return static_cast<int>(m_texture.getSize().x);
}

int SFMLTexture::getHeight() const {
    return static_cast<int>(m_texture.getSize().y);
}
