#ifndef SHIMERA_OPENGLTEXTURE_HPP
#define SHIMERA_OPENGLTEXTURE_HPP

#include "backend/ITexture.hpp"

namespace shimera {

/**
 * OpenGL Texture implementation owning ITexture interface
 * Creates and manages an OpenGL texture resource with automatic cleanup
 */
class OpenGLTexture final : public ITexture {
    public:
        explicit OpenGLTexture(const int width, const int height);
        ~OpenGLTexture() override;

        OpenGLTexture(const OpenGLTexture&) = delete;
        OpenGLTexture& operator=(const OpenGLTexture&) = delete;
        OpenGLTexture(OpenGLTexture&&) = delete;
        OpenGLTexture& operator=(OpenGLTexture&&) = delete;

        void bind(int slot = 0) override;
        void unbind() override;
        [[nodiscard]] unsigned int getNativeHandle() const override;
        [[nodiscard]] int getWidth() const override;
        [[nodiscard]] int getHeight() const override;

    private:
        unsigned int m_textureId;
        int m_width;
        int m_height;
};

}

#endif //SHIMERA_OPENGLTEXTURE_HPP
