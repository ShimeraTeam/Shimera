#pragma once

#include <cstdint>
#include <optional>

#include "GLTexture.hpp"
#include "common/Color.inl"

namespace shimera {

/**
 * The framebuffer currently being drawn into, as a raw GL name.
 * Callers that need to restore a target they did not create use these two.
 */
[[nodiscard]] uint32_t currentDrawFramebuffer();
void bindFramebuffer(uint32_t handle);

// Clears just the depth (and stencil) of whatever is bound, leaving colour alone.
void clearDepth();

/**
 * An offscreen render target, one colour texture, plus depth as either a samplable
 * texture or a plain renderbuffer.
 */
class GLFramebuffer {
    public:
        GLFramebuffer(int width, int height, bool samplableDepth = false);
        ~GLFramebuffer();

        GLFramebuffer(const GLFramebuffer&) = delete;
        GLFramebuffer& operator=(const GLFramebuffer&) = delete;
        GLFramebuffer(GLFramebuffer&&) = delete;
        GLFramebuffer& operator=(GLFramebuffer&&) = delete;

        void bind() const;
        static void clear(Color color = Color::black());
        void resize(int width, int height);

        [[nodiscard]] GLTexture& color() { return *m_color; }
        [[nodiscard]] const GLTexture& color() const { return *m_color; }
        /* nullptr unless this framebuffer was built with samplableDepth.
        * could be a const method, but i'll wait the end of the POC before doing that */
        [[nodiscard]] GLTexture* depth() { return m_depth ? &*m_depth : nullptr; }

        [[nodiscard]] uint32_t handle() const { return m_fbo; }
        [[nodiscard]] int width() const { return m_width; }
        [[nodiscard]] int height() const { return m_height; }

    private:
        uint32_t m_fbo = 0;
        uint32_t m_rbo = 0;
        std::optional<GLTexture> m_color;
        std::optional<GLTexture> m_depth;
        int m_width = 0;
        int m_height = 0;
        bool m_samplableDepth = false;
};

}
