#ifndef SHIMERA_OPENGLFRAMEBUFFER_HPP
#define SHIMERA_OPENGLFRAMEBUFFER_HPP

#include <memory>

#include "backend/IFrameBuffer.hpp"
#include "backend/opengl/OpenGLTexture.hpp"

namespace shimera {

/**
 * OpenGL Framebuffer implementation
 * Wraps an OpenGL framebuffer object with an attached color texture
 */
class OpenGLFramebuffer final : public IFrameBuffer {
    public:
        explicit OpenGLFramebuffer(int width, int height);
        ~OpenGLFramebuffer() override;

        OpenGLFramebuffer(const OpenGLFramebuffer&) = delete;
        OpenGLFramebuffer& operator=(const OpenGLFramebuffer&) = delete;
        OpenGLFramebuffer(OpenGLFramebuffer&&) = delete;
        OpenGLFramebuffer& operator=(OpenGLFramebuffer&&) = delete;

        void bind() override;
        void unbind() override;
        void clear(shimera::Color color) override;
        ITexture& getTexture() override;
        void resize(int width, int height) override;
        [[nodiscard]] int getWidth() const override;
        [[nodiscard]] int getHeight() const override;
        void* getNativeRenderTarget() override;

    private:
        unsigned int m_fbo;
        unsigned int m_rbo;
        std::unique_ptr<OpenGLTexture> m_texture;
        int m_width;
        int m_height;
};

}

#endif //SHIMERA_OPENGLFRAMEBUFFER_HPP
