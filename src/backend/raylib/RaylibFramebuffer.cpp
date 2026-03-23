#include "backend/raylib/RaylibFramebuffer.hpp"
#include <stdexcept>
#include <string>
#include <cstdint>
#include <rlgl.h>

RaylibFramebuffer::RaylibFramebuffer(const int width, const int height) : m_width(width), m_height(height), m_renderTexture({0}) {
    RaylibFramebuffer::resize(width, height);
}

void RaylibFramebuffer::bind() {
    BeginTextureMode(m_renderTexture);
}

void RaylibFramebuffer::unbind() {
    EndTextureMode();
}

void RaylibFramebuffer::clear(shimera::Color color) {
    Color raylibColor = {
        static_cast<unsigned char>(color.m_r * 255),
        static_cast<unsigned char>(color.m_g * 255),
        static_cast<unsigned char>(color.m_b * 255),
        static_cast<unsigned char>(color.m_a * 255)
    };

    ClearBackground(raylibColor);
}

ITexture& RaylibFramebuffer::getTexture() {
    return *m_texture;
}

void RaylibFramebuffer::resize(int width, int height) {
    m_width = width;
    m_height = height;

    UnloadRenderTexture(m_renderTexture);
    m_renderTexture = LoadRenderTexture(width, height);
    if (m_renderTexture.texture.id == 0) {
        throw std::runtime_error("Failed to resize Raylib framebuffer to " + 
                                 std::to_string(width) + "x" + std::to_string(height));
    }
    m_texture = std::make_unique<RaylibTexture>(m_renderTexture.texture);
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
