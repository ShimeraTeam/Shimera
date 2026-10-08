#pragma once

#include "GL/GLFramebuffer.hpp"
#include "hosts/IHostTarget.hpp"

namespace shimera {

/**
 * Host target for a plain OpenGL context.
 *
 * Worth noticing: this file includes no GLFW header and the target links no GLFW. It
 * only needs a current GL context, so it serves GLFW, SDL in GL mode, GLUT, or a context
 * someone else made. "GlfwTarget" is a name, not a dependency.
 */
class GlfwTarget final : public IHostTarget {
    public:
        GlfwTarget(int width, int height, bool samplableDepth = false);

        void begin() override;
        void end() override;

        [[nodiscard]] GLTexture& getColor() override { return m_framebuffer.color(); }
        [[nodiscard]] GLTexture* getDepth() override { return m_framebuffer.depth(); }

        void resize(int width, int height) override;
        [[nodiscard]] int getWidth() const override { return m_framebuffer.width(); }
        [[nodiscard]] int getHeight() const override { return m_framebuffer.height(); }

    protected:
        // The "native" object is Shimera's own framebuffer here.
        void* nativeHandle() override { return &m_framebuffer; }

    private:
        GLFramebuffer m_framebuffer;
        uint32_t m_previousFramebuffer = 0;
        int m_previousViewport[4] = {0, 0, 0, 0};
};

}
