#include "backend/opengl/OpenGLTexture.hpp"

#include <GL/glew.h>
#include <glUtils.h>
#include "backend/opengl/OpenGLTexture.hpp"

using shimera::OpenGLTexture;

OpenGLTexture::OpenGLTexture(const int width, const int height, Format format)
    : m_textureId(0), m_width(width), m_height(height) {
    GLC(glGenTextures(1, &m_textureId));
    GLC(glBindTexture(GL_TEXTURE_2D, m_textureId));

    if (format == Format::Depth) {
        GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
        0,GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    } else {
        GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
            0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

OpenGLTexture::~OpenGLTexture() {
    if (m_textureId != 0) {
        GLC(glDeleteTextures(1, &m_textureId));
    }
}

void OpenGLTexture::bind(int slot) {
    GLC(glActiveTexture(GL_TEXTURE0 + slot));
    GLC(glBindTexture(GL_TEXTURE_2D, m_textureId));
}

void OpenGLTexture::unbind() {
    GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int OpenGLTexture::getNativeHandle() const {
    return m_textureId;
}

int OpenGLTexture::getWidth() const {
    return m_width;
}

int OpenGLTexture::getHeight() const {
    return m_height;
}
