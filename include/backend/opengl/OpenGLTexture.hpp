#ifndef SHIMERA_OPENGL_OPENGLTEXTURE_HPP
#define SHIMERA_OPENGL_OPENGLTEXTURE_HPP

#include "backend/ITexture.hpp"

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
    unsigned int textureId;
    int width;
    int height;
};

#endif //SHIMERA_OPENGL_OPENGLTEXTURE_HPP
