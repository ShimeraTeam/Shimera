#include <GL/glew.h>
#include <glUtils.h>
#include "backend/raylib/RaylibTexture.hpp"

RaylibTexture::RaylibTexture(const Texture& texture) : texture(texture) {}

void RaylibTexture::bind(const int slot) {
    GLC(glActiveTexture(GL_TEXTURE0 + slot));
    GLC(glBindTexture(GL_TEXTURE_2D, texture.id));
}

void RaylibTexture::unbind() {
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int RaylibTexture::getNativeHandle() const {
    return texture.id;
}

int RaylibTexture::getWidth() const {
    return texture.width;
}

int RaylibTexture::getHeight() const {
    return texture.height;
}
