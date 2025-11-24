//
// Created by leoma on 24/11/2025.
//

#include "Framebuffer.h"

#include <iostream>
#include <ostream>
#include <GL/glew.h>

#include "glUtils.h"


Framebuffer::Framebuffer(int w, int h)
    : width(w), height(h),fbo(0), texture(0), rbo(0) {
    // Make the framebuffer
    GLC(glGenFramebuffers(1, &fbo));
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

    // Make the texture that the framebuffer will render to
    GLC(glGenTextures(1, &texture));
    GLC(glBindTexture(GL_TEXTURE_2D, texture));
    GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
        0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, texture, 0));

    /* Make the renderbuffer object that will hold the depth and stencil buffers
    for the framebuffer to use */
    GLC(glGenRenderbuffers(1, &rbo));
    GLC(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
    GLC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
        w, h));
    GLC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, rbo));

    GLC(if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;)

    GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

Framebuffer::~Framebuffer() {
    GLC(glDeleteFramebuffers(1, &fbo));
    GLC(glDeleteTextures(1, &texture));
    GLC(glDeleteRenderbuffers(1, &rbo));
}

void Framebuffer::bind() const {
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
}

void Framebuffer::unbind() const {
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

unsigned int Framebuffer::getTexture() const {
    return texture;
}
