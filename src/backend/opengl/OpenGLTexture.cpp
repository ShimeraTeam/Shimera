#include <GL/glew.h>
#include <glUtils.h>
#include "backend/opengl/OpenGLTexture.hpp"

OpenGLTexture::OpenGLTexture(const int width, const int height)
    : textureId(0), width(width), height(height) {
    GLC(glGenTextures(1, &textureId));
    GLC(glBindTexture(GL_TEXTURE_2D, textureId));
    GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
        0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

OpenGLTexture::~OpenGLTexture() {
    if (textureId != 0) {
        GLC(glDeleteTextures(1, &textureId));
    }
}

void OpenGLTexture::bind(const int slot) {
    GLC(glActiveTexture(GL_TEXTURE0 + slot));
    GLC(glBindTexture(GL_TEXTURE_2D, textureId));
}

void OpenGLTexture::unbind() {
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int OpenGLTexture::getNativeHandle() const {
    return textureId;
}

int OpenGLTexture::getWidth() const {
    return width;
}

int OpenGLTexture::getHeight() const {
    return height;
}
