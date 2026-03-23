#include <GL/glew.h>
#include <glUtils.h>
#include "backend/raylib/RaylibTexture.hpp"

RaylibTexture::RaylibTexture(const Texture& texture) : m_texture(texture) {}

void RaylibTexture::bind(const int slot) {
    GLC(glActiveTexture(GL_TEXTURE0 + slot));
    GLC(glBindTexture(GL_TEXTURE_2D, m_texture.id));
}

void RaylibTexture::unbind() {
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int RaylibTexture::getNativeHandle() const {
    return m_texture.id;
}

int RaylibTexture::getWidth() const {
    return m_texture.width;
}

int RaylibTexture::getHeight() const {
    return m_texture.height;
}
