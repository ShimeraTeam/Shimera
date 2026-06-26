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
    RaylibFramebuffer(int width, int height, bool samplableDepth);
    ~RaylibFramebuffer() override = default;

    void bind() override;
    void unbind() override;
    void clear(Color color) override;
    ITexture& getTexture() override;
    ITexture& getDepthTexture() override;
    void resize(int width, int height) override;
    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;
    void* getNativeRenderTarget() override;

private:
    RenderTexture2D m_renderTexture;
    std::unique_ptr<RaylibTexture> m_texture;
    std::unique_ptr<RaylibTexture> m_depthTexture;
    int m_width, m_height;
    bool m_samplableDepth;
};

}

#endif //SHIMERA_RAYLIBFRAMEBUFFER_HPP