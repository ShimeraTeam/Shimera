// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without using any GPU knowledge
// Copyright (C) 2025-2026 The Shimera Authors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glUtils.h>

#include <stdexcept>

#include "backend/opengl/OpenGLFramebuffer.hpp"

using shimera::ITexture;
using shimera::OpenGLFramebuffer;

OpenGLFramebuffer::OpenGLFramebuffer(const int width, const int height, const bool samplableDepth)
    : m_fbo(0), m_rbo(0), m_texture(nullptr), m_depthTexture(nullptr),
        m_width(width), m_height(height), m_samplableDepth(samplableDepth)
{
    OpenGLFramebuffer::resize(width, height);
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    if (m_rbo != 0) {
        GLC(glDeleteRenderbuffers(1, &m_rbo));
    }
    if (m_fbo != 0) {
        GLC(glDeleteFramebuffers(1, &m_fbo));
    }
}

void OpenGLFramebuffer::bind() {
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
}

void OpenGLFramebuffer::unbind() {
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGLFramebuffer::clear(const Color color) {
    GLC(glClearColor(color.m_r, color.m_g, color.m_b, color.m_a));
    GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

ITexture& OpenGLFramebuffer::getTexture() {
    return *m_texture;
}

ITexture& OpenGLFramebuffer::getDepthTexture() {
    if (!m_depthTexture)
        throw std::runtime_error("This Framebuffer does not have a depth texture");
    return *m_depthTexture;
}

void OpenGLFramebuffer::resize(const int width, const int height) {

    if (glfwGetCurrentContext() == nullptr) {
        throw std::runtime_error("No OpenGL context current");
    }

    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Framebuffer size too small");
    }

    GLint maxTextureSize = 0;
    GLint maxRenderbufferSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);

    if (width > maxTextureSize || height > maxTextureSize) {
        throw std::runtime_error("Framebuffer texture size too large");
    }

    if (width > maxRenderbufferSize || height > maxRenderbufferSize) {
        throw std::runtime_error("Framebuffer renderbuffer size too large");
    }

    m_width = width;
    m_height = height;

    if (m_fbo == 0) {
        GLC(glGenFramebuffers(1, &m_fbo));
    }
    if (m_rbo == 0) {
        GLC(glGenRenderbuffers(1, &m_rbo));
    }

    m_texture = std::make_unique<OpenGLTexture>(width, height);
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, m_texture->getNativeHandle(), 0));

    if (m_samplableDepth) {
        m_depthTexture = std::make_unique<OpenGLTexture>(width, height, OpenGLTexture::Format::Depth);
        GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_TEXTURE_2D, m_depthTexture->getNativeHandle(), 0));
    } else {
        GLC(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
        GLC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
        GLC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER, m_rbo));
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        throw std::runtime_error("OpenGL framebuffer is not complete");
    }

    GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

int OpenGLFramebuffer::getWidth() const {
    return m_width;
}

int OpenGLFramebuffer::getHeight() const {
    return m_height;
}

void* OpenGLFramebuffer::getNativeRenderTarget() {
    return nullptr;
}
