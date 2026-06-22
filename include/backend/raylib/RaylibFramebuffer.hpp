#ifndef SHIMERA_RAYLIBFRAMEBUFFER_HPP
#define SHIMERA_RAYLIBFRAMEBUFFER_HPP

#include <memory>
#include <raylib.h>
#include "backend/IFrameBuffer.hpp"
#include "RaylibTexture.hpp"

namespace shimera {

/**
 * Raylib Framebuffer implementation
 * Wraps around Raylib RenderTexture to provide framebuffer functionality
 */
class RaylibFramebuffer final : public IFrameBuffer {
public:
    RaylibFramebuffer(int width, int height);
    ~RaylibFramebuffer() override = default;

    void bind() override;
    void unbind() override;
    void clear(shimera::Color color) override;
    ITexture& getTexture() override;
    void resize(int width, int height) override;
    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;
    void* getNativeRenderTarget() override;

private:
    RenderTexture2D m_renderTexture;
    std::unique_ptr<RaylibTexture> m_texture;
    int m_width, m_height;
};

}

#endif //SHIMERA_RAYLIBFRAMEBUFFER_HPP