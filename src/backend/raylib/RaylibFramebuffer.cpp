#include "backend/raylib/RaylibFramebuffer.hpp"
#include <stdexcept>
#include <string>
#include <cstdint>
#include <rlgl.h>

using shimera::ITexture;
using shimera::RaylibFramebuffer;
using shimera::RaylibTexture;

RaylibFramebuffer::RaylibFramebuffer(const int width, const int height, bool samplableDepth) :
    m_renderTexture({0}), m_width(width), m_height(height), m_samplableDepth(samplableDepth)
{
    RaylibFramebuffer::resize(width, height);
}

void RaylibFramebuffer::bind() {
    BeginTextureMode(m_renderTexture);
}

void RaylibFramebuffer::unbind() {
    EndTextureMode();
}

void RaylibFramebuffer::clear(shimera::Color color) {
    const ::Color raylibColor = {
        .r = static_cast<unsigned char>(color.m_r * 255),
        .g = static_cast<unsigned char>(color.m_g * 255),
        .b = static_cast<unsigned char>(color.m_b * 255),
        .a = static_cast<unsigned char>(color.m_a * 255)
    };

    ClearBackground(raylibColor);
}

ITexture& RaylibFramebuffer::getTexture() {
    return *m_texture;
}

ITexture& RaylibFramebuffer::getDepthTexture() {
    if (!m_depthTexture)
        throw std::runtime_error("This Framebuffer does not have a depth texture");
    return *m_depthTexture;
}

void RaylibFramebuffer::resize(int width, int height) {
    m_width = width;
    m_height = height;

    UnloadRenderTexture(m_renderTexture);

    if (m_samplableDepth) {
        m_renderTexture = {0};
        m_renderTexture.id = rlLoadFramebuffer();
        rlEnableFramebuffer(m_renderTexture.id);

        m_renderTexture.texture.id = rlLoadTexture(nullptr, width, height,
            PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        m_renderTexture.texture.width = width;
        m_renderTexture.texture.height = height;
        m_renderTexture.texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        m_renderTexture.texture.mipmaps = 1;

        m_renderTexture.depth.id = rlLoadTextureDepth(width, height, false);
        m_renderTexture.depth.width = width;
        m_renderTexture.depth.height = height;

        rlFramebufferAttach(m_renderTexture.id, m_renderTexture.texture.id,
            RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(m_renderTexture.id, m_renderTexture.depth.id,
            RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);
        rlDisableFramebuffer();
    } else {
        m_renderTexture = LoadRenderTexture(width, height);
    }

    if (m_renderTexture.texture.id == 0) {
        throw std::runtime_error("Failed to resize Raylib framebuffer to " + 
                                 std::to_string(width) + "x" + std::to_string(height));
    }
    m_texture = std::make_unique<RaylibTexture>(m_renderTexture.texture);
    if (m_samplableDepth)
        m_depthTexture = std::make_unique<RaylibTexture>(m_renderTexture.depth);
}

int RaylibFramebuffer::getWidth() const {
    return m_width;
}

int RaylibFramebuffer::getHeight() const {
    return m_height;
}

void* RaylibFramebuffer::getNativeRenderTarget() {
    return &m_renderTexture;
}
