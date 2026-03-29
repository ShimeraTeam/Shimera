#ifndef SHIMERA_OPENGL_OPENGLBACKEND_HPP
#define SHIMERA_OPENGL_OPENGLBACKEND_HPP

#include "backend/IBackend.hpp"

/**
 * OpenGL backend implementation
 * Creates OpenGL-specific implementations of framework interfaces
 */
class OpenGLBackend final : public IBackend {
    public:
        OpenGLBackend() = default;
        ~OpenGLBackend() override = default;

        IFrameBuffer* createFrameBuffer(int width, int height) override;
        IPostProccessor* createPostProcessor(const std::string& vert, const std::string& frag) override;
        ITexture* createTexture(int width, int height) override;
        IShader* createShader(const std::string& vert, const std::string& frag) override;
};

#endif //SHIMERA_OPENGL_OPENGLBACKEND_HPP
