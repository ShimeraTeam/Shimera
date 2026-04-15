#include "backend/raylib/RaylibFramebuffer.hpp"
#include <stdexcept>
#include <string>
#include <cstdint>
#include <rlgl.h>

RaylibFramebuffer::RaylibFramebuffer(const int width, const int height) : m_width(width), m_height(height), renderTexture({0}) {
    RaylibFramebuffer::resize(width, height);
}

void RaylibFramebuffer::bind() {
    BeginTextureMode(renderTexture);
}

void RaylibFramebuffer::unbind() {
    EndTextureMode();
}

void RaylibFramebuffer::clear(shimera::Color color) {
    Color raylibColor = {
        static_cast<unsigned char>(color.r * 255),
        static_cast<unsigned char>(color.g * 255),
        static_cast<unsigned char>(color.b * 255),
        static_cast<unsigned char>(color.a * 255)
    };

    ClearBackground(raylibColor);
}

ITexture& RaylibFramebuffer::getTexture() {
    return *texture;
}

void RaylibFramebuffer::resize(int width, int height) {
    m_width = width;
    m_height = height;

    UnloadRenderTexture(renderTexture);
    renderTexture = LoadRenderTexture(width, height);
    if (renderTexture.texture.id == 0) {
        throw std::runtime_error("Failed to resize Raylib framebuffer to " + 
                                 std::to_string(width) + "x" + std::to_string(height));
    }
    texture = std::make_unique<RaylibTexture>(renderTexture.texture);
}

int RaylibFramebuffer::getWidth() const {
    return m_width;
}

int RaylibFramebuffer::getHeight() const {
    return m_height;
}

void* RaylibFramebuffer::getNativeRenderTarget() {
    return &renderTexture;
}
